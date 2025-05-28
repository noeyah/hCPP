#include "TimeUtil.h"
#include <format>

namespace hlib
{
	Time::steady_time Time::NowSteady()
	{
		return std::chrono::steady_clock::now();
	}

	Time::steady_time Time::TimePointAfter(steady_time baseTime, milliseconds afterMs)
	{
		return baseTime + afterMs;
	}

	Time::steady_time Time::TimePointAfter(steady_time baseTime, uint64_t afterMs)
	{
		return TimePointAfter(baseTime, milliseconds(afterMs));
	}

	Time::steady_time Time::TimePointAfterFromNow(milliseconds afterMs)
	{
		return Time::TimePointAfter(NowSteady(), afterMs);
	}

	Time::steady_time Time::TimePointAfterFromNow(uint64_t afterMs)
	{
		return TimePointAfter(NowSteady(), afterMs);
	}

	bool Time::IsExpired(steady_time expiredTime)
	{
		return NowSteady() >= expiredTime;
	}

	bool Time::IsExpired(steady_time startTime, milliseconds timeoutMs)
	{
		return IsExpired(startTime + timeoutMs);
	}

	bool Time::IsExpired(steady_time startTime, uint64_t timeoutMs)
	{
		return IsExpired(startTime, milliseconds(timeoutMs));
	}

	Time::system_time Time::NowSystem()
	{
		return std::chrono::system_clock::now();
	}

	std::string Time::TimeToString()
	{
		return TimeToString(NowSystem());
	}

	std::string Time::TimeToString(system_time time)
	{
		const auto localTime = std::chrono::zoned_time(GetCurrentZone(), time);
		return std::format("{:%Y-%m-%d %H:%M:%S}", localTime);
	}

	const std::chrono::time_zone* Time::GetCurrentZone()
	{
		static const std::chrono::time_zone* timeZone = std::chrono::current_zone();
		return timeZone;
	}
}
