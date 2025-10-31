#include "JobQueue.h"

namespace hlib::task
{
	void JobQueue::Stop()
	{
		queue_.RequestStop();
	}

	void JobQueue::Push(IJob::SharedPtr job)
	{
		queue_.Push(std::move(job));
	}

	IJob::SharedPtr JobQueue::Pop()
	{
		IJob::SharedPtr job;
		if (!queue_.WaitPop(job))
			return nullptr;

		return job;
	}
}