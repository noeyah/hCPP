#include "Server.h"
#include <memory>
#include "Server/ServerSession.h"

void Server::Init(hlib::NetConfig& config)
{
	m_pThreadPool = std::make_shared<hlib::ThreadPool>(m_jobQueue, 1);
	m_pScheduler = std::make_shared<hlib::Scheduler>(m_jobQueue);
	m_pServer = std::make_shared<hlib::NetClient>(config, m_jobQueue);

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

std::shared_ptr<ServerSession> Server::Connect()
{
	if (!m_pServer)
		return {};

	auto pSession = m_pServer->Connect();
	if (!pSession)
		return {};

	return std::static_pointer_cast<ServerSession>(pSession);
}
