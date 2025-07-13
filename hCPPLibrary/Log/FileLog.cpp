#include "FileLog.h"
#include <format>
#include <filesystem>
#include <iostream>

#include "Util/Macro.h"
#include "Util/TimeUtil.h"
#include "Util/StringUtil.h"

namespace hlib::log
{
	FileLog::FileLog(const std::string& fileName)
	{
		stream_.open(fileName, std::ios::app);
		ASSERT_CRASH(stream_.is_open());
	}

	void FileLog::Write(const LogMessage& log)
	{
		if (!stream_.is_open() || !stream_.good())
		{
			std::cerr << "ERROR : log file stream" << std::endl;
			return;
		}

		auto time = Time::TimeToString(log.time);
		stream_ << std::format("[{}]", time);

		stream_ << std::format("[{:<5}]", LogLevelToString(log.logLevel));
		stream_ << std::format("[{}]", log.threadId);

		auto fullPath = log.location.file_name();
		std::filesystem::path filePath(fullPath);
		auto fileNamePath = filePath.filename();
		stream_ << std::format("[{}({}:{})]", fileNamePath.string(), log.location.line(), log.location.column());

		stream_ << " " << log.message << std::endl; // flush
	}
}