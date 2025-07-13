#pragma once
#include <chrono>
#include <thread>
#include <source_location>
#include <string>
#include "Util/TimeUtil.h"

namespace hlib::log
{
	enum class LogLevel
	{
		None,
		Debug,
		Info,
		Warning,
		Error
	};

	struct LogMessage
	{
		LogLevel logLevel;
		std::chrono::system_clock::time_point time;
		std::thread::id threadId;
		std::source_location location;
		std::string message;

		LogMessage(LogLevel lv,
				   std::string msg,
				   const std::source_location& sl = std::source_location::current())
			: logLevel(lv),
			time(Time::NowSystem()),
			threadId(std::this_thread::get_id()),
			message(std::move(msg)),
			location(sl) 
		{
		}

		LogMessage() = default;
	};

	static const char* LogLevelToString(LogLevel level)
	{
		switch (level)
		{
		case hlib::log::LogLevel::Debug:
			return "DEBUG";
		case hlib::log::LogLevel::Info:
			return "INFO";
		case hlib::log::LogLevel::Warning:
			return "WARN";
		case hlib::log::LogLevel::Error:
			return "ERROR";
		case hlib::log::LogLevel::None:
		default:
			return "NONE";
		}
	}
}