#pragma once
#include <cstdint>
#include <memory>

namespace hlib::task
{
	class IJob
	{
	public:
		using SharedPtr = std::shared_ptr<IJob>;

		virtual ~IJob() = default;
		virtual void Run() = 0;
	};

	class IJobQueue
	{
	public:
		virtual ~IJobQueue() = default;
		virtual void Stop() = 0;
		virtual void Push(std::shared_ptr<IJob> job) = 0;
		virtual IJob::SharedPtr Pop() = 0;
	};
}
