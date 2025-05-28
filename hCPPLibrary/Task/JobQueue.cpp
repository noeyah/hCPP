#include "JobQueue.h"

namespace hlib::task
{
	void JobQueue::Stop()
	{
		queue_.RequestStop();
	}

	void JobQueue::Push(std::shared_ptr<IJob> job)
	{
		queue_.Push(std::move(job));
	}

	std::shared_ptr<IJob> JobQueue::Pop()
	{
		std::shared_ptr<IJob> job;
		if (!queue_.WaitPop(job))
			return nullptr;

		return job;
	}
}