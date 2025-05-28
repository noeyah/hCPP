#pragma once
#include <memory>

#include "NetService.h"
#include "Network/NetConfig.h"

namespace hlib::task
{
	class IJobQueue;
}

namespace hlib::net
{
	class NetClient : public NetService
	{
	public:
		NetClient(NetClientConfig config, task::IJobQueue& jobQueue, SessionManager::SessionFactory factory);
		~NetClient();

	protected:
		virtual bool InitSocket() final;
		virtual void CloseSocket() final;

	private:
		void Connect();

	private:
		NetClientConfig clientConfig_;
	};

}