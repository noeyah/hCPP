#include "MainServer.h"
#include <memory>
#include "Client/ClientSession.h"

void MainServer::Init(hlib::NetServerConfig config)
{
	m_pThreadPool = std::make_shared<hlib::ThreadPool>(m_jobQueue, 1);
	m_pScheduler = std::make_shared<hlib::Scheduler>(m_jobQueue);
	m_pServer = std::make_shared<hlib::NetServer>(config, m_jobQueue);
	m_pRoom = std::make_shared<Room>();
}

void MainServer::Start()
{
	if (!m_pServer->Start<ClientSession>())
	{
		LOG_ERROR("server start failed");
		return;
	}
	LOG_INFO("Server Start");
}

void MainServer::Stop()
{
	m_pServer->Stop();
	m_pScheduler->Stop();
	m_pThreadPool->Stop();
}
