#pragma once
#include "core.h"

class Server : public Singleton<Server>
{
	core::JobQueue m_jobQueue{};
	std::shared_ptr<core::ThreadPool> m_pThreadPool{};
	std::shared_ptr<core::Scheduler> m_pScheduler{};
	std::shared_ptr<core::NetClient> m_pServer{};

public:
	Server() = default;

	void Init(core::NetClientConfig config);
	void Start();
	void Stop();

	std::shared_ptr<core::Scheduler> GetScheduler() { return m_pScheduler; }
};

