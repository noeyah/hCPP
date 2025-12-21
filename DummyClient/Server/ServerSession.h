#pragma once
#include <memory>
#include <span>
#include <cstddef>
#include <atomic>
#include <cstdint>
#include <mutex>
#include "core.h"

class Dummy;

class ServerSession : public hlib::Session
{
	std::weak_ptr<Dummy> m_wpDummy{};

public:
	std::shared_ptr<ServerSession> GetSharedPtr()
	{
		return std::static_pointer_cast<ServerSession>(shared_from_this());
	}

	void SetDummy(std::weak_ptr<Dummy> wpDummy) { m_wpDummy = wpDummy; }
	std::shared_ptr<Dummy> GetDummy() { return m_wpDummy.lock(); }

protected:
	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnReceive(uint16_t packetId, std::span<const std::byte> packet) override;

	
};

