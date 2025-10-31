#pragma once
#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cassert>
#include <type_traits>
#include <cstddef>
#include <cstdint>
#include <google/protobuf/message.h>
#include "core.h"
#include "PacketID.h"

class PacketRegister
{
public:
	static void Initialize();

	template <typename TPacket>
	static uint16_t GetID()
	{
		auto it = typeToId_.find(std::type_index(typeid(TPacket)));
		if (it != typeToId_.end())
		{
			return it->second;
		}

		return static_cast<uint16_t>(packet::PacketID::None);
	}

	template <typename TPacket>
		requires std::is_base_of_v<google::protobuf::Message, TPacket>
	static std::shared_ptr<core::PacketBuffer> Serialize(const TPacket& sendPacket)
	{
		size_t packetSize = sendPacket.ByteSizeLong();
		size_t bufferSize = core::HEADER_SIZE + packetSize;

		core::PacketHeader header;
		header.size = static_cast<uint16_t>(bufferSize);
		header.id = GetID<TPacket>();

		auto sendBuffer = std::make_shared<core::PacketBuffer>(bufferSize);
		sendBuffer->Write(reinterpret_cast<const std::byte*>(&header), core::HEADER_SIZE);

		bool success = sendPacket.SerializeToArray(sendBuffer->WritePtr(), static_cast<int>(packetSize));
		assert(success);
		sendBuffer->CommitWrite(packetSize);

		return sendBuffer;
	}

private:
	template <typename TPacket>
	static void RegisterPacketID(packet::PacketID packetId)
	{
		typeToId_[std::type_index(typeid(TPacket))] = static_cast<uint16_t>(packetId);
	}

private:
	PacketRegister() = delete;
	~PacketRegister() = delete;

private:
	inline static std::unordered_map<std::type_index, uint16_t> typeToId_;
};
