#pragma once
#include <memory>

#include "NetService.h"
#include "Network/NetConfig.h"

namespace hlib
{
	class IJobQueue;

	class NetClient : public NetService
	{
		NetClientConfig m_clientConfig;

	public:
		NetClient(NetClientConfig config, IJobQueue& jobQueue);
		~NetClient();

	protected:
		virtual bool InitSocket() final;
		virtual void CloseSocket() final;

	private:
		void Connect();
	};

}