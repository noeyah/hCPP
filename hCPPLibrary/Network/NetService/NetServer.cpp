#include "NetServer.h"
#include "Network/Connection/Listener.h"
#include "Log.h"

namespace hlib
{
	NetServer::NetServer(NetServerConfig config, IJobQueue& jobQueue)
		: NetService(config, jobQueue),
		m_serverConfig(config)
	{
		m_pListener = std::make_shared<Listener>(
			m_dispatcher.GetHandle(), 
			[this](SOCKET s, const SOCKADDR_IN* addr) { m_sessionMgr.CreateAndStart(s, addr); });
	}

	NetServer::~NetServer()
	{
		Stop();
	}

	bool NetServer::InitSocket()
	{
		if (!m_pListener->Start(m_address, m_serverConfig.acceptCount, m_serverConfig.backlog))
		{
			return false;
		}

		return true;
	}

	void NetServer::CloseSocket()
	{
		if (m_pListener)
		{
			m_pListener->Stop();
			m_pListener.reset();
		}
	}
}