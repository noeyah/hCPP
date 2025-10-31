#include "ClientSession.h"
#include "Packet/PacketID.h"
#include "Packet/PacketDispatcher.h"

void ClientSession::OnConnected()
{
	LOG_DEBUG("session({}) connected", GetSessionId());
}

void ClientSession::OnDisconnected()
{
	auto session = GetSharedPtr();
	packetDispatcher_.OnDisconnected(session);
}

void ClientSession::OnReceive(uint16_t packetId, std::span<const std::byte> packet)
{
	auto session = GetSharedPtr();
	packetDispatcher_.Dispatch(session, static_cast<packet::PacketID>(packetId), packet);
}
