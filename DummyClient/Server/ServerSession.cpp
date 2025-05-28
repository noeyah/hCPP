#include "ServerSession.h"
#include "Packet/PacketID.h"
#include "Packet/PacketDispatcher.h"

ServerSession::ServerSession(PacketDispatcher& dispatcher) : packetDispatcher_(dispatcher)
{
}

void ServerSession::OnConnected(uint64_t sid)
{
	sid_ = sid;
	auto session = GetSharedPtr();
	packetDispatcher_.OnConnected(session);
}

void ServerSession::OnDisconnected()
{
	auto session = GetSharedPtr();
	packetDispatcher_.OnDisconnected(session);
}

void ServerSession::OnReceive(uint16_t packetId, std::span<const std::byte> packet)
{
	auto session = GetSharedPtr();
	packetDispatcher_.Dispatch(session, static_cast<packet::PacketID>(packetId), packet);
}
