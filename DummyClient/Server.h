#pragma once
#include <Task/JobQueue.h>
#include <Task/ThreadPool.h>
#include <Task/Scheduler.h>
#include <Network/NetConfig.h>
#include <Network/NetService/NetClient.h>
#include "Packet/PacketHandler.h"
#include "Packet/PacketDispatcher.h"

class Server
{
public:
	Server(hlib::net::NetClientConfig config);

	void Start();
	void Stop();

private:
	hlib::task::JobQueue jobQueue_;
	hlib::task::ThreadPool threadPool_;
	hlib::task::Scheduler scheduler_;

	PacketHandler packetHandler_;
	PacketDispatcher packetDispatcher_;

	hlib::net::NetClient netService_;
};

