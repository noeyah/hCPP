#include "NetServer.h"
#include "Network/Connection/Listener.h"
#include "Log.h"

namespace hlib::net
{
	NetServer::NetServer(NetServerConfig config, task::IJobQueue& jobQueue, SessionManager::SessionFactory factory)
		: NetService(config, jobQueue, std::move(factory)),
		serverConfig_(config), 
		socketPool_(config.socketPoolInitCount, jobQueue)
	{
		listener_ = std::make_shared<Listener>(iocpDispatcher_.GetHandle(), socketPool_, sessionManager_);

		sessionManager_.SetReturnSockCallback([this](SOCKET sock) { 
			this->socketPool_.Return(sock); 
		});
	}

	NetServer::~NetServer()
	{
		Stop();
	}

	bool NetServer::InitSocket()
	{
		if (!listener_->Start(address_, serverConfig_.acceptCount, serverConfig_.backlog))
		{
			return false;
		}

		return true;
	}

	void NetServer::CloseSocket()
	{
		if (listener_)
		{
			listener_->Stop();
			listener_.reset();
		}
	}
}