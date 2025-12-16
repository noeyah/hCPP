#pragma once
#include <memory>
#include <string>
#include <cstdint>
#include "core.h"
#include "Client/ClientSession.h"

class User
{
	uint64_t m_sessionID;
	std::string m_name;
	std::weak_ptr<ClientSession> m_wpSession;

public:
	User(uint64_t sessionId, const std::string& name, std::weak_ptr<ClientSession> session)
		: m_sessionID(sessionId), m_name(name), m_wpSession(session)
	{
	}

	void Send(const std::shared_ptr<hlib::PacketBuffer>& buffer) const
	{
		if (auto session = m_wpSession.lock())
		{
			session->Send(buffer);
		}
	}

	uint64_t GetSessionId() const { return m_sessionID; }
	const std::string& GetName() const { return m_name; }
	std::weak_ptr<ClientSession> GetWeakPtr() const { return m_wpSession; }
	
};