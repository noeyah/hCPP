#include "WorkerThread.h"
#include "Util/Macro.h"
#include "Memory/Memory.h"

namespace hlib
{
	WorkerThread::WorkerThread(std::function<void(std::atomic_bool&)> workFunc, 
							   std::function<void()> notifyFunc) 
		: workFunc_(std::move(workFunc)), 
		notifyFunc_(std::move(notifyFunc))
	{
		assert(workFunc_);
	}

	WorkerThread::~WorkerThread()
	{
		Stop();
	}

	void WorkerThread::Start(size_t count)
	{
		if (isRunning_.exchange(true))
			return;

		workerCount_.store(count);

		std::lock_guard lock(mtx_);

		threads_.reserve(count);
		for (uint32_t i = 0; i < count; i++)
		{
			threads_.emplace_back(std::thread([this]() {
				this->workFunc_(this->isRunning_);
			}));
		}
	}

	void WorkerThread::StartAsCurrentThread()
	{
		if (isRunning_.exchange(true))
			return;

		workerCount_.store(1);

		workFunc_(isRunning_);
	}

	void WorkerThread::Stop()
	{
		if (!isRunning_.exchange(false))
			return;

		if (notifyFunc_)
			notifyFunc_();

		Join();
		workerCount_.store(0);
	}

	void WorkerThread::Join()
	{
		std::vector<std::thread> threads;
		{
			std::lock_guard lock(mtx_);
			threads_.swap(threads);
		}

		for (auto& t : threads)
		{
			if (t.joinable())
				t.join();
		}
	}
}