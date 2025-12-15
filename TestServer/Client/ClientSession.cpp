#include "ClientSession.h"
#include "Packet/PacketID.h"
#include "Packet/PacketDispatcher.h"

void ClientSession::OnConnected()
{
	LOG_DEBUG("session({}) connected", GetID());
}

void ClientSession::OnDisconnected()
{
	auto session = GetSharedPtr();
	PacketDispatcher::Instance()->OnDisconnected(session);
}

void ClientSession::OnReceive(uint16_t packetId, std::span<const std::byte> packet)
{
	auto session = GetSharedPtr();
	PacketDispatcher::Instance()->Dispatch(session, static_cast<packet::PacketID>(packetId), packet);
}
