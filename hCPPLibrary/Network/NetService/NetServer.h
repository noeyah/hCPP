#pragma once
#include <memory>

#include "NetService.h"
#include "Network/NetConfig.h"
#include "Network/Socket/SocketPool.h"

namespace hlib::task
{
	class IJobQueue;
}

namespace hlib::net
{
	class Listener;

	class NetServer : public NetService
	{
	public:
		NetServer(NetServerConfig config, task::IJobQueue& jobQueue, SessionManager::SessionFactory factory);
		~NetServer();

	protected:
		virtual bool InitSocket() final;
		virtual void CloseSocket() final;

	private:
		NetServerConfig serverConfig_;
		SocketPool socketPool_;
		std::shared_ptr<Listener> listener_;
	};

}