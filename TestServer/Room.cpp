#include "Room.h"

void Room::JoinUser(uint64_t sessionId, const std::string& name, std::weak_ptr<ClientSession> session)
{
	std::lock_guard lock(mtx_);
	users_.emplace(sessionId, User(sessionId, name, session));
}

void Room::LeaveUser(uint64_t sessionId)
{
	std::lock_guard lock(mtx_);
	users_.erase(sessionId);
}

void Room::ForEachUser(const std::function<void(const User&)>& fn) const
{
	std::lock_guard lock(mtx_);
	for (const auto& pair : users_)
	{
		fn(pair.second);
	}
}

void Room::Broadcast(std::shared_ptr<core::PacketBuffer> buffer, uint64_t excludeId)
{
	auto users = GetSessionPtrs(excludeId);
	for (const auto& user : users)
	{
		user->Send(buffer);
	}
}

bool Room::IsMember(uint64_t sessionId)
{
	std::lock_guard lock(mtx_);
	return users_.contains(sessionId);
}

core::Vector<std::shared_ptr<ClientSession>> Room::GetSessionPtrs(uint64_t excludeId) const
{
	core::Vector<std::shared_ptr<ClientSession>> ptrs;

	std::lock_guard lock(mtx_);
	ptrs.reserve(users_.size());

	for (const auto& pair : users_)
	{
		if (pair.first == excludeId)
			continue;

		if (auto session = pair.second.GetWeakPtr().lock())
		{
			ptrs.push_back(session);
		}
	}

	return ptrs;
}
