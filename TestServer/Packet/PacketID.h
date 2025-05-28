// 이 파일은 스크립트로 생성됨
// 위치 : Solution/Packet/jinja2/cpp

#pragma once
#include <cstdint>

namespace packet
{
	enum class PacketID : uint16_t
	{
		None = 0,
		ChatNot = 1,
		ChatReq = 2,
		ChatRes = 3,
		JoinRoomNot = 4,
		JoinRoomReq = 5,
		JoinRoomRes = 6,
		LeaveRoomNot = 7,
		LeaveRoomReq = 8,
		LeaveRoomRes = 9,
	};
}