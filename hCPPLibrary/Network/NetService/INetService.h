#pragma once
#include <memory>
#include "Network/NetCommon.h"

namespace hlib::net
{
	class Session;

	class INetService
	{
	public:
		virtual ~INetService() = default;

		virtual bool Start() = 0;
		virtual void Stop() = 0;
	};
}