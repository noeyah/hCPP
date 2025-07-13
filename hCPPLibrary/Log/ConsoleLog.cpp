#include "ConsoleLog.h"
#include <syncstream>
#include <iostream>
#include <format>
#include <sstream>
#include <filesystem>

#include "Util/TimeUtil.h"
#include "Util/StringUtil.h"

namespace hlib::log
{
	void ConsoleLog::Write(const LogMessage& log)
	{
		std::osyncstream stream(std::cout);

		auto time = Time::TimeToString(log.time);
		stream << std::format("[{}]", time);

		stream << std::format("[{:<5}]", LogLevelToString(log.logLevel));
		stream << std::format("[{}]", log.threadId);

#ifdef _LOG_CONSOLE_FILENAME
		auto fullPath = log.location.file_name();
		std::filesystem::path filePath(fullPath);
		auto fileNamePath = filePath.filename();
		stream << std::format("[{}({}:{})]", fileNamePath.string(), log.location.line(), log.location.column());
#endif // _LOG_CONSOLE_FILENAME

		stream << " " << log.message << std::endl;
	}

}