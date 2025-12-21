#pragma once
#include <memory>
#include <type_traits>
#include <atomic>
#include <google/protobuf/message.h>
#include "bt.h"
#include "ServerSession.h"
#include "Packet/PacketRegister.h"

enum class DummyState
{
	None,
	Connected,
	Disconnected
};

// 균등확률로 하면 방을 너무 자주 나가서, 가중치를 주려는 용도. (Think에서 정해줌)
enum class DummyTaskInRoom
{
	None,
	Chat,
	LeaveRoom,
	Wait,
};

class Dummy : public std::enable_shared_from_this<Dummy>
{
	int m_id{};

	std::weak_ptr<ServerSession> m_wpSession;
	std::shared_ptr<hlib::BehaviorTree> m_pBT;

	std::atomic<DummyState> m_eState{ DummyState::None };
	std::atomic_bool m_bInRoom{ false };
	std::atomic<DummyTaskInRoom> m_eNextTask{ DummyTaskInRoom::None };

public:
	Dummy(int id) : m_id(id) {}

	int GetID() { return m_id; }
	DummyTaskInRoom GetNextTask() { return m_eNextTask.load(std::memory_order_relaxed); }
	
	bool IsConnected() const { return m_eState.load(std::memory_order_relaxed) == DummyState::Connected; }
	bool IsInRoom() const { return m_bInRoom.load(std::memory_order_relaxed); }

	void SetState(DummyState eState) { m_eState.store(eState, std::memory_order_relaxed); }
	void SetRoom(bool bRoom) { m_bInRoom.store(bRoom, std::memory_order_relaxed); }
	void SetNextTask(DummyTaskInRoom type) { m_eNextTask.store(type, std::memory_order_relaxed); }

	void Init();
	void Update();

	bool Connect();
	
	template <typename TPacket>
		requires std::is_base_of_v<google::protobuf::Message, TPacket>
	bool Send(const TPacket& packet)
	{
		if (auto pSession = m_wpSession.lock())
		{
			return pSession->Send(PacketRegister::Serialize(packet));
		}
		return false;
	}
};

