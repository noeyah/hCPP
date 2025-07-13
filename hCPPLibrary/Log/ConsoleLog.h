#pragma once
#include "LogMessage.h"
#include "ILogStream.h"

namespace hlib::log
{
	class ConsoleLog : public ILogStream
	{
	public:
		ConsoleLog() = default;
		~ConsoleLog() = default;

		virtual void Write(const LogMessage& log) override;
	};
}