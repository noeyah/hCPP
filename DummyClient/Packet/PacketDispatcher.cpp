#include "PacketDispatcher.h"
#include <cassert>

using namespace packet;

void PacketDispatcher::Dispatch(SessionPtr& session, PacketID packetId, std::span<const std::byte> data)
{
	auto it = m_handleMap.find(packetId);
	assert(it != m_handleMap.end());
	it->second(session, data);
}
