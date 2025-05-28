#pragma once
#include <memory>
#include <string>
#include <cstdint>
#include <Network/Buffer/PacketBuffer.h>
#include "Client/ClientSession.h"

class User
{
public:
	User(uint64_t sessionId, const std::string& name, std::weak_ptr<ClientSession> session)
		: sessionId_(sessionId), name_(name), session_(session)
	{
	}

	void Send(const std::shared_ptr<hlib::net::PacketBuffer>& buffer) const
	{
		if (auto session = session_.lock())
		{
			session->Send(buffer);
		}
	}

	uint64_t GetSessionId() const { return sessionId_; }
	const std::string& GetName() const { return name_; }
	std::weak_ptr<ClientSession> GetWeakPtr() const { return session_; }

private:
	uint64_t sessionId_;
	std::string name_;
	std::weak_ptr<ClientSession> session_;
};