#include "Server.h"
#include <memory>
#include "Server/ServerSession.h"

Server::Server(core::NetClientConfig config)
	: threadPool_(jobQueue_, 3),
	scheduler_(jobQueue_),
	packetHandler_(scheduler_),
	packetDispatcher_(packetHandler_),
	netService_(config,
				jobQueue_,
				[this]() -> std::shared_ptr<ServerSession> { return core::MakeSharedPtr<ServerSession>(this->packetDispatcher_); })
{
}

void Server::Start()
{
	if (!netService_.Start())
	{
		LOG_ERROR("net client start failed");
		return;
	}
	LOG_INFO("Connected");
}

void Server::Stop()
{
	netService_.Stop();
	scheduler_.Stop();
	threadPool_.Stop();
}
