#pragma once
#include <memory>
#include "Network/NetCommon.h"

namespace hlib
{
	class Session;

	// TODO 삭제예정
	class INetService
	{
	public:
		virtual ~INetService() = default;

		virtual bool Start() = 0;
		virtual void Stop() = 0;
	};
}