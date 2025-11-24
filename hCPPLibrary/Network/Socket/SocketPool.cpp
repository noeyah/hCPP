#include "SocketPool.h"

#include "Util/Macro.h"
#include "Task/IJobQueue.h"
#include "Task/TaskUtil.h"
#include "Network/Socket/SocketUtil.h"
#include "Lock/LockGuard.h"

namespace hlib::net
{
	SocketPool::SocketPool(size_t initCount, task::IJobQueue& jobQueue) : jobQueue_(jobQueue)
	{
		pool_.reserve(initCount);
		for (size_t i = 0; i < initCount; i++)
		{
			SOCKET sock = sock::Create();
			ASSERT_CRASH(sock != INVALID_SOCKET);
			pool_.emplace_back(sock);
		}
	}

	SocketPool::~SocketPool()
	{
		Lock lock(spin_);
		for (SOCKET sock : pool_)
		{
			sock::Close(sock);
		}
		pool_.clear();
	}

	bool SocketPool::TryGet(SOCKET* sock)
	{
		Lock lock(spin_);
		if (pool_.empty())
		{
			CheckRefillTrigger();
			return false;
		}

		*sock = pool_.back();
		pool_.pop_back();

		CheckRefillTrigger();
		return true;
	}

	void SocketPool::Return(SOCKET sock)
	{
		if (sock == INVALID_SOCKET)
			return;

		Lock lock(spin_);
		pool_.emplace_back(sock);
	}

	void SocketPool::CheckRefillTrigger()
	{
		if (refillPending_.load())
			return;

		if (pool_.size() > MIN_POOL_COUNT)
			return;

		bool expected = false;
		if (refillPending_.compare_exchange_strong(expected, true))
		{
			task::PushJob(jobQueue_, [this]() {
				this->Refill();
				this->refillPending_.store(false);
				});
		}
	}

	void SocketPool::Refill()
	{
		std::vector<SOCKET> newSocket;

		for (uint32_t i = 0; i < REFILL_COUNT; i++)
		{
			SOCKET sock = sock::Create();
			ASSERT_CRASH(sock != INVALID_SOCKET);
			newSocket.emplace_back(sock);
		}

		Lock lock(spin_);
		for (SOCKET sock : newSocket)
		{
			pool_.emplace_back(sock);
		}
	}

}