#pragma once
#include "core.h"
#include "Packet/PacketHandler.h"
#include "Packet/PacketDispatcher.h"

class Server
{
public:
	Server(core::NetClientConfig config);

	void Start();
	void Stop();

private:
	core::JobQueue jobQueue_;
	core::ThreadPool threadPool_;
	core::Scheduler scheduler_;

	PacketHandler packetHandler_;
	PacketDispatcher packetDispatcher_;

	core::NetClient netService_;
};

