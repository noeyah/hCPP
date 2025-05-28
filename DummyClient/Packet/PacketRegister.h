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
#include <Network/NetCommon.h>
#include <Network/Buffer/PacketBuffer.h>
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
	static std::shared_ptr<hlib::net::PacketBuffer> Serialize(const TPacket& sendPacket)
	{
		size_t packetSize = sendPacket.ByteSizeLong();
		size_t bufferSize = hlib::net::HEADER_SIZE + packetSize;

		hlib::net::PacketHeader header;
		header.size = static_cast<uint16_t>(bufferSize);
		header.id = GetID<TPacket>();

		auto sendBuffer = std::make_shared<hlib::net::PacketBuffer>(bufferSize);
		sendBuffer->Write(reinterpret_cast<const std::byte*>(&header), hlib::net::HEADER_SIZE);

		assert(sendPacket.SerializeToArray(sendBuffer->WritePtr(), static_cast<int>(packetSize)));
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
