#pragma once
#include <memory>
#include <cstdint>
#include <atomic>
#include <vector>
#include "WinCommon.h"
#include "Lock/SpinLock.h"

namespace hlib::task
{
	class IJobQueue;
}

namespace hlib::net
{
	class SocketPool
	{
	private:
		inline static constexpr size_t MIN_POOL_COUNT = 10;
		inline static constexpr size_t REFILL_COUNT = 10;

	public:
		SocketPool(size_t initCount, task::IJobQueue& jobQueue);
		~SocketPool();

		bool TryGet(SOCKET* sock);
		void Return(SOCKET sock);

	private:
		void CheckRefillTrigger();
		void Refill();

	private:
		SpinLock spin_;
		std::vector<SOCKET> pool_;
		std::atomic_bool refillPending_ = false;
		hlib::task::IJobQueue& jobQueue_;
	};
}