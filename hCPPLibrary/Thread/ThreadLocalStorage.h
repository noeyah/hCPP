#pragma once
#include <cstdint>
#include <thread>
#include <string>
#include <chrono>
#include <atomic>
#include <memory>
#include <deque>

namespace hlib
{
	class TLS
	{
	public:
		struct ThreadInfo
		{
			ThreadInfo() : threadId(std::this_thread::get_id()), startTime(std::chrono::steady_clock::now())
			{
				workerId = ++lastWorkerId;
			}
			~ThreadInfo() = default;

			uint32_t workerId = 0;
			std::thread::id threadId;

			std::chrono::steady_clock::time_point startTime;
		};

	private:
		TLS() = delete;
		TLS(const TLS&) = delete;
		TLS& operator=(const TLS&) = delete;
		TLS(TLS&&) = delete;
		TLS& operator=(TLS&&) = delete;

	private:
		inline static std::atomic_uint32_t lastWorkerId = 0;

		// thread local storage
	public:
		inline static thread_local ThreadInfo info;

		// SharedSpinLock
		inline static thread_local uint32_t writeRecursionCount = 0;
		inline static thread_local uint32_t readRecursionCount = 0;
	};
}