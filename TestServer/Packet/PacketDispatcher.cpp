﻿#include "PacketDispatcher.h"
#include <cassert>

using namespace packet;

PacketDispatcher::PacketDispatcher(PacketHandler& handler) : packetHandler_(handler)
{
	Initialize();
}

void PacketDispatcher::Dispatch(SessionPtr& session, PacketID packetId, std::span<const std::byte> data)
{
	auto it = handleMap_.find(packetId);
	ASSERT_CRASH(it != handleMap_.end());
	it->second(session, data);
}

void PacketDispatcher::OnDisconnected(SessionPtr& session)
{
	packetHandler_.OnDisconnected(session);
}
