#pragma once
#include "core.h"

class ServerSession;

class Server : public Singleton<Server>
{
	hlib::JobQueue m_jobQueue{};
	std::shared_ptr<hlib::ThreadPool> m_pThreadPool{};
	std::shared_ptr<hlib::Scheduler> m_pScheduler{};
	std::shared_ptr<hlib::NetClient> m_pServer{};

public:
	Server() = default;

	void Init(hlib::NetConfig& config);
	void Start();
	void Stop();

	std::shared_ptr<hlib::Scheduler> GetScheduler() { return m_pScheduler; }
	std::shared_ptr<ServerSession> Connect();
};

