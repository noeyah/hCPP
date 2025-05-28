#pragma once
#include <thread>
#include <cstdint>
#include <atomic>
#include "Thread/WorkerThread.h"

namespace hlib::task
{
	class IJobQueue;

	class ThreadPool
	{
	public:
		ThreadPool(IJobQueue& jobQueue, uint32_t threadCount = std::thread::hardware_concurrency());
		~ThreadPool();

		void Stop();

	private:
		void Work(std::atomic_bool& running);

	private:
		uint32_t threadCount_;
		WorkerThread thread_;
		IJobQueue& queue_;
	};
}
