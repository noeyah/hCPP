#pragma once
#include <memory>

#include "NetService.h"
#include "Network/NetConfig.h"

namespace hlib
{
	class IJobQueue;
	class Session;

	class NetClient : public NetService
	{
	public:
		NetClient(NetConfig config, IJobQueue& jobQueue);
		~NetClient();
		std::shared_ptr<Session> Connect();

	protected:
		virtual bool InitSocket() final;
		virtual void CloseSocket() final;
	};

}