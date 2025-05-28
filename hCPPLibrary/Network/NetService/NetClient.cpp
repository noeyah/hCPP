#include "NetClient.h"
#include "Util/Macro.h"
#include "Network/Socket/SocketUtil.h"
#include "Network/Connection/Session.h"
#include "Log.h"

namespace hlib::net
{
	NetClient::NetClient(NetClientConfig config, task::IJobQueue& jobQueue, SessionManager::SessionFactory factory)
		: NetService(config, jobQueue, std::move(factory)),
		clientConfig_(config)
	{
	}

	NetClient::~NetClient()
	{
		Stop();
	}

	bool NetClient::InitSocket()
	{
		for (size_t i = 0; i < clientConfig_.connectCount; i++)
		{
			Connect();
		}

		LOG_INFO("Connect {}", clientConfig_.connectCount);
		return true;
	}

	void NetClient::CloseSocket()
	{
	}

	void NetClient::Connect()
	{
		SOCKET socket = sock::Create();
		ASSERT_CRASH(socket != INVALID_SOCKET);

		auto session = sessionManager_.Create(socket, &address_);
		if (session)
		{
			session->Connect();
		}
	}
}