#pragma once
#include <cstdint>
#include <atomic>

#include "WinCommon.h"
#include "INetService.h"
#include "Network/Core/IocpDispatcher.h"
#include "Network/Connection/SessionManager.h"
#include "Network/NetConfig.h"

namespace hlib::task
{
	class IJobQueue;
}

namespace hlib::net
{
	class NetService : public INetService
	{
	public:
		NetService(NetConfig config, task::IJobQueue& jobQueue, SessionManager::SessionFactory factory);
		virtual ~NetService() = default;

		virtual bool Start() final;
		virtual void Stop() final;

	protected:
		virtual bool InitSocket() = 0;
		virtual void CloseSocket() = 0;

	protected:
		SOCKADDR_IN address_{};
		NetConfig netConfig_;
		std::atomic_bool isRunning_ = false;

		IocpDispatcher iocpDispatcher_;
		SessionManager sessionManager_;
	};

}