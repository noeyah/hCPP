#pragma once
#include <functional>
#include <unordered_map>
#include <span>
#include <memory>
#include <cstddef>
#include <type_traits>
#include <google/protobuf/message.h>
#include <Util/Macro.h>
#include "PacketID.h"
#include "Server/ServerSession.h"
#include "PacketHandler.h"
#include "PacketEnums.h"

using SessionPtr = std::shared_ptr<ServerSession>;
using HandleFunc = std::function<void(SessionPtr&, std::span<const std::byte>)>;

class PacketDispatcher
{
public:
	PacketDispatcher(PacketHandler& handler);
	void Dispatch(SessionPtr& session, packet::PacketID packetId, std::span<const std::byte> data);
	void OnConnected(SessionPtr& session);
	void OnDisconnected(SessionPtr& session);

private:
	// 초기화 함수 PacketDispatcherInit.cpp
	void Initialize();

	template <typename TPacket>
		requires std::is_base_of_v<google::protobuf::Message, TPacket>
	void RegisterPacket(packet::PacketID packetId)
	{
		handleMap_[packetId] = [packetId, &handler = this->packetHandler_](SessionPtr& session, std::span<const std::byte> body)
		{
			TPacket data;
			if (!data.ParseFromArray(body.data(), static_cast<int>(body.size())))
			{
				CRASH("Packet Parse Failed. packetId : {}", packetId);
			}

			handler.OnReceivePacket(session, data);
		};
	}

private:
	std::unordered_map<packet::PacketID, HandleFunc> handleMap_;
	PacketHandler& packetHandler_;
};