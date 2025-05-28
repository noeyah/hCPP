#include "ThreadPool.h"
#include "IJobQueue.h"
#include "Job.h"

namespace hlib::task
{
	ThreadPool::ThreadPool(IJobQueue& jobQueue, uint32_t threadCount)
		: threadCount_(threadCount), 
		queue_(jobQueue),
		thread_([this](std::atomic_bool& running) { this->Work(running); },
				[this]() {this->queue_.Stop(); })
	{
		thread_.Start(threadCount_);
	}

	ThreadPool::~ThreadPool()
	{
		Stop();
	}

	void ThreadPool::Stop()
	{
		thread_.Stop();
	}

	void ThreadPool::Work(std::atomic_bool& running)
	{
		while (running.load())
		{
			auto job = queue_.Pop();
			if (!job)
			{
				break;
			}
			job->Run();
		}
	}

}