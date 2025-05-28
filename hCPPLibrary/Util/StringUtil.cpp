#include "StringUtil.h"

namespace hlib
{
	std::string ConvertToUtf8(const std::wstring& wstr)
	{
		if (wstr.empty())
			return {};

		int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string strTo(size - 1, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strTo[0], size, nullptr, nullptr);
		return strTo;
	}

	std::wstring ConvertToWString(const std::string& str)
	{
		if (str.empty())
			return {};

		int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		std::wstring wstr(size - 1, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
		return wstr;
	}

}