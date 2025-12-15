#pragma once
#include <memory>
#include <mutex>

#include "WinCommon.h"
#include "Memory/Container.h"

namespace hlib
{
	class PacketBuffer;

	class SendBuffer
	{
	private:
		Deque<std::shared_ptr<PacketBuffer>> m_queue;
		mutable std::recursive_mutex m_lock;
		std::atomic_bool m_bSending{ false };

	public:
		SendBuffer() = default;
		~SendBuffer() { Clear(); }

		bool Begin() { return !m_bSending.exchange(true); }
		void End() { m_bSending.store(false); }

		void Push(std::shared_ptr<PacketBuffer>& buffer);
		void PrepareSend(OUT Vector<WSABUF>& wsaBufs, size_t maxSize, OUT size_t& sendCount, OUT size_t& sendBytes);
		void CompleteSend(size_t count);

		void Clear();
		bool IsEmpty() const;
		size_t GetQueueSize() const;

		// 복사/이동 불가
		SendBuffer(const SendBuffer&) = delete;
		SendBuffer& operator=(const SendBuffer&) = delete;
		SendBuffer(SendBuffer&&) = delete;
		SendBuffer& operator=(SendBuffer&&) = delete;
	};
}