#include "MainServer.h"
#include <memory>
#include <Log.h>
#include <Memory/Allocator.h>
#include "Client/ClientSession.h"

MainServer::MainServer(hlib::net::NetServerConfig config)
	: threadPool_(jobQueue_),
	scheduler_(jobQueue_),
	netService_(config, 
				jobQueue_,
				[this]() -> std::shared_ptr<ClientSession> { return hlib::MakeSharedPtr<ClientSession>(this->packetDispatcher_); }),
	packetHandler_(room_),
	packetDispatcher_(packetHandler_)
{
}

void MainServer::Start()
{
	if (!netService_.Start())
	{
		LOG_ERROR("server start failed");
		return;
	}
	LOG_INFO("Server Start");
}

void MainServer::Stop()
{
	netService_.Stop();
	scheduler_.Stop();
	threadPool_.Stop();
}
