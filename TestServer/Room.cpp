#include "Room.h"

void Room::JoinUser(uint64_t sessionId, const std::string& name, std::weak_ptr<ClientSession> session)
{
	std::lock_guard lock(m_lock);
	m_users.emplace(sessionId, std::make_shared<User>(sessionId, name, session));
}

void Room::LeaveUser(uint64_t sessionId)
{
	std::lock_guard lock(m_lock);
	m_users.erase(sessionId);
}

void Room::ForEachUser(const std::function<void(const User&)>& fn) const
{
	std::lock_guard lock(m_lock);
	for (const auto& pair : m_users)
	{
		fn(*pair.second);
	}
}

void Room::Broadcast(std::shared_ptr<hlib::PacketBuffer> buffer, uint64_t excludeId)
{
	std::vector<std::shared_ptr<User>> users{};
	{
		std::lock_guard lock(m_lock);
		users.reserve(m_users.size());

		for (const auto& [id, pUser] : m_users)
		{
			if (id == excludeId)
				continue;
			users.emplace_back(pUser);
		}
	}

	for (const auto& pUser : users)
	{
		pUser->Send(buffer);
	}
}

bool Room::IsMember(uint64_t sessionId)
{
	std::lock_guard lock(m_lock);
	return m_users.contains(sessionId);
}
