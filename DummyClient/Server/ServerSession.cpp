#include "ServerSession.h"
#include "Packet/PacketID.h"
#include "Packet/PacketDispatcher.h"

void ServerSession::OnConnected()
{
	auto session = GetSharedPtr();
	PacketDispatcher::Instance()->OnConnected(session);
}

void ServerSession::OnDisconnected()
{
	auto session = GetSharedPtr();
	PacketDispatcher::Instance()->OnDisconnected(session);
}

void ServerSession::OnReceive(uint16_t packetId, std::span<const std::byte> packet)
{
	auto session = GetSharedPtr();
	PacketDispatcher::Instance()->Dispatch(session, static_cast<packet::PacketID>(packetId), packet);
}
