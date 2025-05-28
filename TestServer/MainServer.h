#pragma once
#include <Task/JobQueue.h>
#include <Task/ThreadPool.h>
#include <Task/Scheduler.h>
#include <Network/NetConfig.h>
#include <Network/NetService/NetServer.h>
#include "Packet/PacketHandler.h"
#include "Packet/PacketDispatcher.h"
#include "Room.h"

class MainServer
{
public:
	MainServer(hlib::net::NetServerConfig config);

	void Start();
	void Stop();

private:
	hlib::task::JobQueue jobQueue_;
	hlib::task::ThreadPool threadPool_;
	hlib::task::Scheduler scheduler_;

	Room room_;
	
	PacketHandler packetHandler_;
	PacketDispatcher packetDispatcher_;

	hlib::net::NetServer netService_;

};

