#pragma once
#include "Log/Logger.h"

#define HLIB_LOG_INTERNAL(logLv, fmt, ...) \
hlib::log::Logger::Instance().Log(logLv, std::source_location::current(), fmt, ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...)		HLIB_LOG_INTERNAL(hlib::log::LogLevel::Debug,	fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)		HLIB_LOG_INTERNAL(hlib::log::LogLevel::Info,	fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)		HLIB_LOG_INTERNAL(hlib::log::LogLevel::Warning,	fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)		HLIB_LOG_INTERNAL(hlib::log::LogLevel::Error,	fmt, ##__VA_ARGS__)
