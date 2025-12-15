#pragma once
#include <cstdint>
#include <string>
#include "WinCommon.h"

namespace hlib
{
	struct NetConfig
	{
		std::string name;
		std::string ip;
		uint16_t port;
		size_t ioThreadCount = 1;
	};

	struct NetServerConfig : public NetConfig
	{
		size_t acceptCount;
		int32_t backlog = SOMAXCONN;
	};

	struct NetClientConfig : public NetConfig
	{
		size_t connectCount = 1;
	};
}