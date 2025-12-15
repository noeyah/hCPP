#pragma once
#include <memory>
#include <tuple>
#include <utility>
#include <functional>

#include "IJobQueue.h"
#include "Job.h"
#include "Memory/Allocator.h"

namespace hlib
{
	template<typename F>
	void PushJob(IJobQueue& queue, F&& func)
	{
		auto job = hlib::MakeSharedPtr<Job<F>>(std::forward<F>(func));
		queue.Push(std::move(job));
	}
}