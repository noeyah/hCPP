#pragma once
#include <memory>

#include "NetService.h"
#include "Network/NetConfig.h"

namespace hlib
{
	class IJobQueue;
	class Listener;

	class NetServer : public NetService
	{
		NetServerConfig m_serverConfig;
		std::shared_ptr<Listener> m_pListener;

	public:
		NetServer(NetServerConfig config, IJobQueue& jobQueue);
		~NetServer();

	protected:
		virtual bool InitSocket() final;
		virtual void CloseSocket() final;
		
	};

}