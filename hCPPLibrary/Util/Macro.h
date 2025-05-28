#pragma once

#include <string>
#include <format>
#include <source_location>
#include <cassert>
#include <utility>

namespace hlib::macro
{
	void _Crash(const std::string& message,
				const std::source_location& location = std::source_location::current());

	template <typename... Args>
	inline void CrashHelper(const std::source_location& location, std::format_string<Args...> fmt, Args&&... args)
	{
		std::string crashMessage = std::format(fmt, std::forward<Args>(args)...);
		_Crash(crashMessage, location);
	}
}

#define CRASH(...) hlib::macro::CrashHelper(std::source_location::current(), __VA_ARGS__)


#define ASSERT_CRASH(expr)								\
	do													\
	{													\
		if (!(expr))									\
		{												\
			CRASH("Assertion Failed : {}", #expr);		\
		}												\
	} while(0)
