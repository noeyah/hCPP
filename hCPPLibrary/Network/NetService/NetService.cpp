#include "NetService.h"
#include "Task/IJobQueue.h"
#include "Util/Macro.h"
#include "Network/WinsockSetup.h"

namespace hlib::net
{
	NetService::NetService(NetConfig config, task::IJobQueue& jobQueue, SessionManager::SessionFactory factory)
		: netConfig_(config), 
		iocpDispatcher_(jobQueue), 
		sessionManager_(iocpDispatcher_.GetHandle(), std::move(factory))
	{
		WinsockSetup::Instance();

		address_.sin_family = AF_INET;
		if (inet_pton(AF_INET, config.ip.c_str(), &address_.sin_addr) <= 0)
		{
			CRASH("ip error");
			return;
		}

		address_.sin_port = htons(config.port);
	}

	bool NetService::Start()
	{
		ASSERT_CRASH(netConfig_.ioThreadCount > 0);

		if (isRunning_.exchange(true))
			return false;

		iocpDispatcher_.Start(netConfig_.ioThreadCount);

		if (!InitSocket())
		{
			return false;
		}

		return true;
	}

	void NetService::Stop()
	{
		if (!isRunning_.exchange(false))
			return;

		CloseSocket();
		//sessionManager_.Close();
		iocpDispatcher_.Stop();
	}

}