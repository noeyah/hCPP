#pragma once
#include <memory>
#include <span>
#include <cstddef>
#include <cstdint>
#include <atomic>
#include "core.h"
#include <mutex>

class PacketDispatcher;

class ClientSession : public core::Session
{
public:
	ClientSession(PacketDispatcher& dispatcher) : packetDispatcher_(dispatcher)
	{
	}

protected:
	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnReceive(uint16_t packetId, std::span<const std::byte> packet) override;

private:
	std::shared_ptr<ClientSession> GetSharedPtr()
	{
		return std::static_pointer_cast<ClientSession>(shared_from_this());
	}

private:
	PacketDispatcher& packetDispatcher_;
};

