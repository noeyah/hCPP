#pragma once
#include "core.h"
#include "Packet/PacketHandler.h"
#include "Packet/PacketDispatcher.h"
#include "Room.h"

class MainServer : public Singleton<MainServer>
{
	core::JobQueue m_jobQueue{};
	std::shared_ptr<core::ThreadPool> m_pThreadPool{};
	std::shared_ptr<core::Scheduler> m_pScheduler{};
	std::shared_ptr<core::NetServer> m_pServer{};
	std::shared_ptr<Room> m_pRoom{};

public:
	MainServer() = default;

	void Init(core::NetServerConfig config);
	void Start();
	void Stop();

	Room* GetRoom() { return m_pRoom.get(); }
};

