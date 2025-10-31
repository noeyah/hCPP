#pragma once
#include "IJobQueue.h"
#include "Memory/LockedQueue.h"

namespace hlib::task
{
	class JobQueue : public IJobQueue
	{
	public:
		virtual void Stop() override;
		virtual void Push(IJob::SharedPtr job) override;
		virtual IJob::SharedPtr Pop() override;

	private:
		LockedQueue<IJob::SharedPtr> queue_;
	};
}



