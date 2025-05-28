#include "ClientSession.h"
#include <Log.h>

#include "Packet/PacketID.h"
#include "Packet/PacketDispatcher.h"

void ClientSession::OnConnected(uint64_t sid)
{
	LOG_DEBUG("client({}) connected", sid);
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
