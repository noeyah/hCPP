#include "NetClient.h"
#include "Util/Macro.h"
#include "Network/Socket/SocketUtil.h"
#include "Network/Connection/Session.h"
#include "Log.h"

namespace hlib
{
	NetClient::NetClient(NetConfig config,IJobQueue& jobQueue)
		: NetService(config, jobQueue)
	{
	}

	NetClient::~NetClient()
	{
		Stop();
	}

	bool NetClient::InitSocket()
	{
		/*for (size_t i = 0; i < m_clientConfig.connectCount; i++)
		{
			Connect();
		}

		LOG_INFO("Connect {}", m_clientConfig.connectCount);*/
		return true;
	}

	void NetClient::CloseSocket()
	{
	}

	std::shared_ptr<Session> NetClient::Connect()
	{
		SOCKET socket = sock::Create();
		ASSERT_CRASH(socket != INVALID_SOCKET);

		auto session = m_sessionMgr.Create(socket, &m_address);
		if (session)
		{
			session->ConnectAsync();
		}
		return session;
	}
}