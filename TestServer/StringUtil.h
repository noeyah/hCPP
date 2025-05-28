#pragma once
#include <string>

inline bool RemoveWhitespace(std::string& str)
{
	const std::string whitespace = " \t\n\r\f\v";

	size_t lastCharPost = str.find_last_not_of(whitespace);

	if (lastCharPost == std::string::npos)
	{
		// 제대로 된 문자가 없음
		str.clear();
		return false;
	}
	
	str.erase(lastCharPost + 1);
	return true;
}

inline size_t Utf8Count(const std::string& utf8_str)
{
	size_t count = 0;

	for (size_t i = 0; i < utf8_str.size(); )
	{
		auto c = utf8_str[i];
		size_t clength = 0;

		if ((c & 0x80) == 0x00) clength = 1;		// 0xxxxxxx 1바이트(ASCII)
		else if ((c & 0xE0) == 0xC0) clength = 2;	// 110xxxxx 2바이트
		else if ((c & 0xF0) == 0xE0) clength = 3;	// 1110xxxx 3바이트
		else if ((c & 0xF8) == 0xF0) clength = 4;	// 11110xxx 4바이트
		else
		{
			return 0;
		}

		i += clength;
		count++;
	}

	return count;
}