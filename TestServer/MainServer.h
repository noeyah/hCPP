#pragma once
#include "core.h"
#include "Packet/PacketHandler.h"
#include "Packet/PacketDispatcher.h"
#include "Room.h"

class MainServer
{
public:
	MainServer(core::NetServerConfig config);

	void Start();
	void Stop();

private:
	core::JobQueue jobQueue_;
	core::ThreadPool threadPool_;
	core::Scheduler scheduler_;

	Room room_;
	
	PacketHandler packetHandler_;
	PacketDispatcher packetDispatcher_;

	core::NetServer netService_;

};

