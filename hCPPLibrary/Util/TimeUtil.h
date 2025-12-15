#pragma once
#include <cstdint>
#include <chrono>
#include <string>

namespace hlib
{
	class Time
	{
	public:
		using steady_time = std::chrono::steady_clock::time_point;
		using system_time = std::chrono::system_clock::time_point;
		using milliseconds = std::chrono::milliseconds;

	public:
		// steady_clock
		static steady_time NowSteady();

		static steady_time TimePointAfter(steady_time baseTime, milliseconds afterMs);
		static steady_time TimePointAfter(steady_time baseTime, uint64_t afterMs);
		static steady_time TimePointAfterFromNow(milliseconds afterMs);
		static steady_time TimePointAfterFromNow(uint64_t afterMs);

		static bool IsExpired(steady_time expiredTime);
		static bool IsExpired(steady_time startTime, milliseconds timeoutMs);
		static bool IsExpired(steady_time startTime, uint64_t timeoutMs);

		// system_clock
		static system_time NowSystem();

		static std::string TimeToString();
		static std::string TimeToString(system_time time);

	private:
		static const std::chrono::time_zone* GetCurrentZone();


	private:
		Time() = delete;
		~Time() = delete;
	};
}
