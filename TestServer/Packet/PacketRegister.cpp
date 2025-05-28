// 이 파일은 스크립트로 생성됨
// 위치 : Solution/Packet/jinja2/cpp

#include "PacketRegister.h"

#include <Chat.pb.h>
#include <Enums.pb.h>
#include <Room.pb.h>

using namespace packet;

void PacketRegister::Initialize()
{
	// 초기화 코드 자동으로 생성됨
	RegisterPacketID<ChatNot>(PacketID::ChatNot);
	RegisterPacketID<ChatReq>(PacketID::ChatReq);
	RegisterPacketID<ChatRes>(PacketID::ChatRes);
	RegisterPacketID<JoinRoomNot>(PacketID::JoinRoomNot);
	RegisterPacketID<JoinRoomReq>(PacketID::JoinRoomReq);
	RegisterPacketID<JoinRoomRes>(PacketID::JoinRoomRes);
	RegisterPacketID<LeaveRoomNot>(PacketID::LeaveRoomNot);
	RegisterPacketID<LeaveRoomReq>(PacketID::LeaveRoomReq);
	RegisterPacketID<LeaveRoomRes>(PacketID::LeaveRoomRes);
}