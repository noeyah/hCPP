#include "Server.h"
#include <memory>
#include "Server/ServerSession.h"

void Server::Init(core::NetClientConfig config)
{
	m_pThreadPool = std::make_shared<core::ThreadPool>(m_jobQueue, 1);
	m_pScheduler = std::make_shared<core::Scheduler>(m_jobQueue);
	m_pServer = std::make_shared<core::NetClient>(config, m_jobQueue);

}

void Server::Start()
{
	if (!m_pServer->Start<ServerSession>())
	{
		LOG_ERROR("net client start failed");
		return;
	}
	LOG_INFO("Connected");
}

void Server::Stop()
{
	m_pServer->Stop();
	m_pScheduler->Stop();
	m_pThreadPool->Stop();
}
