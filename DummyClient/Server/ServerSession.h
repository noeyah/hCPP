#pragma once
#include <memory>
#include <span>
#include <cstddef>
#include <atomic>
#include <cstdint>
#include <mutex>
#include "core.h"

class ServerSession : public hlib::Session
{
protected:
	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnReceive(uint16_t packetId, std::span<const std::byte> packet) override;

	std::shared_ptr<ServerSession> GetSharedPtr()
	{
		return std::static_pointer_cast<ServerSession>(shared_from_this());
	}
};

