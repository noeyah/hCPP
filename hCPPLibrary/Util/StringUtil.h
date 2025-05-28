#pragma once
#include <string>
#include <format>
#include <thread>
#include <sstream>
#include "WinCommon.h"

namespace hlib
{
	std::string ConvertToUtf8(const std::wstring& wstr);
	std::wstring ConvertToWString(const std::string& str);
}

template <>
struct std::formatter<std::thread::id> : std::formatter<std::string>
{
	auto format(std::thread::id tid, format_context& ctx) const
	{
		std::ostringstream oss;
		oss << tid;
		return std::formatter<std::string>::format(oss.str(), ctx);
	}
};