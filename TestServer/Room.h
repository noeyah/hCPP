#pragma once
#include <mutex>
#include <memory>
#include <unordered_map>
#include <functional>
#include "core.h"
#include "User.h"

class ClientSession;

class Room
{
	mutable std::recursive_mutex m_lock;
	std::unordered_map<uint64_t, User> m_users;

public:
	void JoinUser(uint64_t sessionId, const std::string& name, std::weak_ptr<ClientSession> session);
	void LeaveUser(uint64_t sessionId);
	void ForEachUser(const std::function<void(const User&)>& fn) const;
	void Broadcast(std::shared_ptr<core::PacketBuffer> buffer, uint64_t excludeId = 0);

	bool IsMember(uint64_t sessionId);

private:
	core::Vector<std::shared_ptr<ClientSession>> GetSessionPtrs(uint64_t excludeId) const;
};

