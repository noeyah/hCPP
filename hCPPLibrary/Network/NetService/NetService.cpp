#include "NetService.h"
#include "Task/IJobQueue.h"
#include "Util/Macro.h"
#include "Network/WinsockSetup.h"

namespace hlib
{
	NetService::NetService(NetConfig config, IJobQueue& jobQueue)
		: m_config(config), 
		m_dispatcher(jobQueue), 
		m_sessionMgr(m_dispatcher.GetHandle())
	{
		WinsockSetup::Instance();

		m_address.sin_family = AF_INET;
		if (inet_pton(AF_INET, config.ip.c_str(), &m_address.sin_addr) <= 0)
		{
			CRASH("ip error");
			return;
		}

		m_address.sin_port = htons(config.port);
	}

	bool NetService::InternalStart()
	{
		ASSERT_CRASH(m_config.ioThreadCount > 0);

		if (m_bRunning.exchange(true))
			return false;

		m_dispatcher.Start(m_config.ioThreadCount);

		if (!InitSocket())
		{
			return false;
		}

		return true;
	}

	void NetService::Stop()
	{
		if (!m_bRunning.exchange(false))
			return;

		CloseSocket();
		//m_sessionMgr.Close();
		m_dispatcher.Stop();
	}
}