#include "Scheduler.h"
#include "IJobQueue.h"

namespace hlib::task
{
	Scheduler::Scheduler(IJobQueue& queue) 
		: queue_(queue),
		thread_([this](std::atomic_bool& running) {this->Work(running); },
				[this]() { this->cv_.notify_all(); })
	{
		thread_.Start();
	}

	Scheduler::~Scheduler()
	{
		Stop();
	}

	void Scheduler::Stop()
	{
		thread_.Stop();
	}

	void Scheduler::Push(time_point time, IJob::SharedPtr job)
	{
		bool needNotify = false;

		{
			std::lock_guard lock(mtx_);

			if (scheduleMap_.empty() || scheduleMap_.begin()->first > time)
				needNotify = true;

			scheduleMap_.emplace(time, std::move(job));
		}

		if (needNotify)
			cv_.notify_one();
	}

	void Scheduler::Work(std::atomic_bool& running)
	{
		std::vector<IJob::SharedPtr> expiredJobs;
		Time::steady_time nextWakeup;

		while (running.load())
		{
			{
				std::unique_lock lock(mtx_);

				if (scheduleMap_.empty())
					nextWakeup = Time::NowSteady() + std::chrono::hours(1);
				else
					nextWakeup = scheduleMap_.begin()->first;

				if (!Time::IsExpired(nextWakeup))
				{
					cv_.wait_until(lock, nextWakeup);
				}

				auto now = hlib::Time::NowSteady();
				auto end_it = scheduleMap_.upper_bound(now);
				expiredJobs.clear();

				for (auto it = scheduleMap_.begin(); it != end_it; it++)
				{
					expiredJobs.emplace_back(std::move(it->second));
				}

				if (!expiredJobs.empty())
					scheduleMap_.erase(scheduleMap_.begin(), end_it);
			}

			for (auto& job : expiredJobs)
			{
				queue_.Push(std::move(job));
			}
		}
	}
}