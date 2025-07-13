#pragma once
#include <fstream>
#include <string>

#include "LogMessage.h"
#include "ILogStream.h"

namespace hlib::log
{
	class FileLog : public ILogStream
	{
	public:
		FileLog(const std::string& fileName = "default.log");
		~FileLog() = default;

		virtual void Write(const LogMessage& log) override;

	private:
		std::ofstream stream_;
	};
}