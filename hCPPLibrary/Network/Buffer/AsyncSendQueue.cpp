#include "AsyncSendQueue.h"

#include "Util/Macro.h"
#include "PacketBuffer.h"

namespace hlib::net
{
	AsyncSendQueue::~AsyncSendQueue()
	{
		Clear();
	}

	void AsyncSendQueue::Push(std::shared_ptr<PacketBuffer>& buffer)
	{
		if (!buffer || buffer->Empty())
			return;

		std::lock_guard lock(mtx_);
		queue_.push_back(std::move(buffer));
	}

	void AsyncSendQueue::PrepareSend(OUT Vector<WSABUF>& wsaBufs, size_t maxSize, OUT size_t& sendCount, OUT size_t& sendBytes)
	{
		std::lock_guard lock(mtx_);
		if (queue_.empty())
		{
			wsaBufs.clear();
			sendCount = 0;
			sendBytes = 0;
			return;
		}

		sendCount = std::min<size_t>(queue_.size(), maxSize);
		sendBytes = 0;

		wsaBufs.resize(sendCount);

		auto it = queue_.begin();
		for (size_t i = 0; i < sendCount; i++)
		{
			const std::shared_ptr<PacketBuffer>& buffer = *it;
			wsaBufs[i].buf = reinterpret_cast<char*>(buffer->Data());
			wsaBufs[i].len = static_cast<ULONG>(buffer->Size());
			sendBytes += wsaBufs[i].len;
			++it;
		}
	}

	void AsyncSendQueue::CompleteSend(size_t sendSize)
	{
		std::lock_guard lock(mtx_);
		
		assert(sendSize <= queue_.size());

		for (size_t i = 0; i < sendSize; i++)
		{
			queue_.pop_front();
		}
	}

	void AsyncSendQueue::Clear()
	{
		std::lock_guard lock(mtx_);
		queue_.clear();
	}

	bool AsyncSendQueue::IsEmpty() const
	{
		std::lock_guard lock(mtx_);
		return queue_.empty();
	}

	size_t AsyncSendQueue::GetQueueSize() const
	{
		std::lock_guard lock(mtx_);
		return queue_.size();
	}

}