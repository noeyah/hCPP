#pragma once
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <map>
#include <cstdint>

#include "Memory/Allocator.h"
#include "Job.h"
#include "Thread/WorkerThread.h"
#include "Util/TimeUtil.h"

namespace hlib::task
{
	class IJobQueue;
	class IJob;

	class Scheduler
	{
		using time_point = std::chrono::steady_clock::time_point;
		using milliseconds = std::chrono::milliseconds;

	public:
		Scheduler(IJobQueue& queue);
		~Scheduler();

		void Stop();

		template<typename F>
		void PushAfter(uint64_t after_ms, F&& func)
		{
			auto job = hlib::MakeSharedPtr<Job<F>>(std::forward<F>(func));
			time_point time = hlib::Time::TimePointAfterFromNow(after_ms);
			Push(time, std::move(job));
		}

		template<typename F>
		void PushAfter(std::chrono::milliseconds after_ms, F&& func)
		{
			auto job = hlib::MakeSharedPtr<Job<F>>(std::forward<F>(func));
			time_point time = hlib::Time::TimePointAfterFromNow(after_ms);
			Push(time, std::move(job));
		}

		template<typename F>
		void PushAt(std::chrono::steady_clock::time_point time, F&& func)
		{
			auto job = hlib::MakeSharedPtr<Job<F>>(std::forward<F>(func));
			Push(time, std::move(job));
		}

	private:
		void Push(time_point time, IJob::SharedPtr job);

		void Work(std::atomic_bool& running);

	private:
		IJobQueue& queue_;
		WorkerThread thread_;
		std::condition_variable cv_;

		std::mutex mtx_;
		std::multimap<time_point, IJob::SharedPtr> scheduleMap_{};
	};
}
