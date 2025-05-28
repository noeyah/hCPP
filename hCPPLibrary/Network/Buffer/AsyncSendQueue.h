#pragma once
#include <memory>
#include <mutex>

#include "WinCommon.h"
#include "Memory/Container.h"

namespace hlib::net
{
	class PacketBuffer;

	class AsyncSendQueue
	{
	public:
		AsyncSendQueue() = default;
		~AsyncSendQueue();

		void Push(std::shared_ptr<PacketBuffer>& buffer);
		void PrepareSend(OUT Vector<WSABUF>& wsaBufs, size_t maxSize, OUT size_t& sendCount, OUT size_t& sendBytes);
		void CompleteSend(size_t sendSize);

		void Clear();
		bool IsEmpty() const;
		size_t GetQueueSize() const;

		// 복사/이동 불가
		AsyncSendQueue(const AsyncSendQueue&) = delete;
		AsyncSendQueue& operator=(const AsyncSendQueue&) = delete;
		AsyncSendQueue(AsyncSendQueue&&) = delete;
		AsyncSendQueue& operator=(AsyncSendQueue&&) = delete;

	private:
		Deque<std::shared_ptr<PacketBuffer>> queue_;
		mutable std::mutex mtx_;
	};
}