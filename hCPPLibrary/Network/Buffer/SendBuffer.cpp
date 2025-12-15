#include "SendBuffer.h"

#include "Util/Macro.h"
#include "PacketBuffer.h"

namespace hlib
{
	void SendBuffer::Push(std::shared_ptr<PacketBuffer>& buffer)
	{
		if (!buffer || buffer->Empty())
			return;

		std::lock_guard lock(m_lock);
		m_queue.emplace_back(buffer);
	}

	void SendBuffer::PrepareSend(OUT Vector<WSABUF>& wsaBufs, size_t maxSize, OUT size_t& sendCount, OUT size_t& sendBytes)
	{
		std::lock_guard lock(m_lock);
		if (m_queue.empty())
		{
			wsaBufs.clear();
			sendCount = 0;
			sendBytes = 0;
			return;
		}

		sendCount = std::min<size_t>(m_queue.size(), maxSize);
		sendBytes = 0;

		wsaBufs.resize(sendCount);

		auto it = m_queue.begin();
		for (size_t i = 0; i < sendCount; i++)
		{
			const std::shared_ptr<PacketBuffer>& buffer = *it;
			wsaBufs[i].buf = reinterpret_cast<char*>(buffer->ReadPos());
			wsaBufs[i].len = static_cast<ULONG>(buffer->DataSize());
			sendBytes += wsaBufs[i].len;
			++it;
		}
	}

	void SendBuffer::CompleteSend(size_t count)
	{
		std::lock_guard lock(m_lock);
		
		ASSERT_CRASH(count <= m_queue.size());

		for (size_t i = 0; i < count; i++)
		{
			m_queue.pop_front();
		}
	}

	void SendBuffer::Clear()
	{
		std::lock_guard lock(m_lock);
		m_queue.clear();
	}

	bool SendBuffer::IsEmpty() const
	{
		std::lock_guard lock(m_lock);
		return m_queue.empty();
	}

	size_t SendBuffer::GetQueueSize() const
	{
		std::lock_guard lock(m_lock);
		return m_queue.size();
	}

}