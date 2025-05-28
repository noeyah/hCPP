// 이 파일은 스크립트로 생성됨
// 위치 : Solution/Packet/jinja2/cpp

#pragma once
#include <string_view>
#include <string>
#include <format>
#include <type_traits>
#include "PacketID.h"

#include <Chat.pb.h>
#include <Enums.pb.h>
#include <Room.pb.h>

// PacketID
inline std::string_view PacketIdToString(packet::PacketID id)
{
	switch (id)
	{
	case packet::PacketID::ChatNot:
		return "ChatNot";
	case packet::PacketID::ChatReq:
		return "ChatReq";
	case packet::PacketID::ChatRes:
		return "ChatRes";
	case packet::PacketID::JoinRoomNot:
		return "JoinRoomNot";
	case packet::PacketID::JoinRoomReq:
		return "JoinRoomReq";
	case packet::PacketID::JoinRoomRes:
		return "JoinRoomRes";
	case packet::PacketID::LeaveRoomNot:
		return "LeaveRoomNot";
	case packet::PacketID::LeaveRoomReq:
		return "LeaveRoomReq";
	case packet::PacketID::LeaveRoomRes:
		return "LeaveRoomRes";
	case packet::PacketID::None:
		return "None";
	default:
		return "UnknownPacketID";
	}
}

// Packet Enums To String 자동 생성

// Defaults
inline std::string_view DefaultsToString(packet::Defaults value)
{
	switch (value)
	{
	case packet::Defaults::DEFAULT_DEFINE:
		return "DEFAULT_DEFINE";
	case packet::Defaults::NAME_MAX_LIMIT:
		return "NAME_MAX_LIMIT";
	case packet::Defaults::MESSAGE_MAX_LIMIT:
		return "MESSAGE_MAX_LIMIT";
	default:
		return "UnknownDefaults";
	}
}
// ErrorCode
inline std::string_view ErrorCodeToString(packet::ErrorCode value)
{
	switch (value)
	{
	case packet::ErrorCode::OK:
		return "OK";
	case packet::ErrorCode::FAIL:
		return "FAIL";
	case packet::ErrorCode::FAIL_ALREADY_IN_ROOM:
		return "FAIL_ALREADY_IN_ROOM";
	case packet::ErrorCode::FAIL_NOT_IN_ROOM:
		return "FAIL_NOT_IN_ROOM";
	case packet::ErrorCode::FAIL_EMPTY_NAME:
		return "FAIL_EMPTY_NAME";
	case packet::ErrorCode::FAIL_EXCEED_NAME_LIMIT:
		return "FAIL_EXCEED_NAME_LIMIT";
	case packet::ErrorCode::FAIL_EXCEED_MESSAGE_LIMIT:
		return "FAIL_EXCEED_MESSAGE_LIMIT";
	case packet::ErrorCode::FAIL_EMPTY_MESSAGE:
		return "FAIL_EMPTY_MESSAGE";
	default:
		return "UnknownErrorCode";
	}
}


// PacketID Formater
template <>
struct std::formatter<packet::PacketID> : std::formatter<std::string>
{
	template <typename FormatContext>
	inline auto format(packet::PacketID id, FormatContext& ctx) const
	{
		std::string_view sv = PacketIdToString(id);
		auto num = static_cast<std::underlying_type_t<packet::PacketID>>(id);
		return std::formatter<std::string>::format(std::format("{}({})", sv, num), ctx);
	}
};

// Packet Enums Formater 자동 생성

// Defaults
template <>
struct std::formatter<packet::Defaults> : std::formatter<std::string>
{
	template <typename FormatContext>
	inline auto format(packet::Defaults value, FormatContext& ctx) const
	{
		std::string_view sv = DefaultsToString(value);
		auto num = static_cast<std::underlying_type_t<packet::Defaults>>(value);
		return std::formatter<std::string>::format(std::format("{}({})", sv, num), ctx);
	}
};
// ErrorCode
template <>
struct std::formatter<packet::ErrorCode> : std::formatter<std::string>
{
	template <typename FormatContext>
	inline auto format(packet::ErrorCode value, FormatContext& ctx) const
	{
		std::string_view sv = ErrorCodeToString(value);
		auto num = static_cast<std::underlying_type_t<packet::ErrorCode>>(value);
		return std::formatter<std::string>::format(std::format("{}({})", sv, num), ctx);
	}
};
