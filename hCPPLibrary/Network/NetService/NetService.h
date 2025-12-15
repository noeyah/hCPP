#pragma once
#include <cstdint>
#include <atomic>

#include "WinCommon.h"
#include "Network/Core/IocpDispatcher.h"
#include "Network/Connection/SessionManager.h"
#include "Network/NetConfig.h"
#include "Network/Connection/Session.h"

namespace hlib
{
	class IJobQueue;

	class NetService
	{
	protected:
		SOCKADDR_IN m_address{};
		NetConfig m_config;
		std::atomic_bool m_bRunning{ false };
		IocpDispatcher m_dispatcher;
		SessionManager m_sessionMgr;

	public:

		NetService(NetConfig config, IJobQueue& jobQueue);
		virtual ~NetService() = default;

		template <typename T>
		bool Start() requires std::derived_from<T, Session>
		{
			m_sessionMgr.SetSessionFactory([]()
			{
				return std::make_shared<T>();
			});
			return InternalStart();
		}

		void Stop();

	protected:
		bool InternalStart();

		virtual bool InitSocket() = 0;
		virtual void CloseSocket() = 0;

	};

}