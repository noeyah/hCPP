#pragma once
#include <memory>
#include <span>
#include <cstddef>
#include <atomic>
#include <cstdint>
#include <Network/Connection/Session.h>
#include <mutex>
#include <Util/TimeUtil.h>

class PacketDispatcher;

class ServerSession : public hlib::net::Session
{
public:
	ServerSession(PacketDispatcher& dispatcher);
	
	uint64_t GetID() const { return sid_; }
protected:
	virtual void OnConnected(uint64_t sid) override;
	virtual void OnDisconnected() override;
	virtual void OnReceive(uint16_t packetId, std::span<const std::byte> packet) override;

	std::shared_ptr<ServerSession> GetSharedPtr()
	{
		return std::static_pointer_cast<ServerSession>(shared_from_this());
	}

public:
	void SetSendTime()
	{
		std::lock_guard lock(timeMtx_);
		lastSendTime_ = hlib::Time::NowSteady();
	}

	void TimeOff()
	{
		std::lock_guard lock(timeMtx_);
		lastSendTime_ = lastSendTime_ + std::chrono::hours(24);
	}

	bool IsSendTimeout()
	{
		std::lock_guard lock(timeMtx_);
		return hlib::Time::IsExpired(lastSendTime_, 5000);
	}

private:
	PacketDispatcher& packetDispatcher_;

	hlib::Time::steady_time lastSendTime_;
	std::mutex timeMtx_;
	uint64_t sid_;
};

