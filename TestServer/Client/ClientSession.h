#pragma once
#include <memory>
#include <span>
#include <cstddef>
#include <cstdint>
#include <atomic>
#include <mutex>
#include "core.h"

class ClientSession : public core::Session
{
protected:
	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnReceive(uint16_t packetId, std::span<const std::byte> packet) override;

private:
	std::shared_ptr<ClientSession> GetSharedPtr()
	{
		return std::static_pointer_cast<ClientSession>(shared_from_this());
	}
};

