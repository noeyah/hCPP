#pragma once
#include "LogMessage.h"

namespace hlib::log
{
	class ILogStream
	{
	public:
		virtual ~ILogStream() = default;

		virtual void Write(const LogMessage& log) = 0;
	};
}