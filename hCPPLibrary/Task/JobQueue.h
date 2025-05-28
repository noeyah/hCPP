#pragma once
#include "IJobQueue.h"
#include "Memory/LockedQueue.h"

namespace hlib::task
{
	class JobQueue : public IJobQueue
	{
	public:
		virtual void Stop() override;
		virtual void Push(std::shared_ptr<IJob> job) override;
		virtual std::shared_ptr<IJob> Pop() override;

	private:
		LockedQueue<std::shared_ptr<IJob>> queue_;
	};
}



