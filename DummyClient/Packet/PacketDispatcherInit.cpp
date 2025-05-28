// 이 파일은 스크립트로 생성됨
// 위치 : Solution/Packet/jinja2/cpp_client

#include "PacketDispatcher.h"

#include <Chat.pb.h>
#include <Enums.pb.h>
#include <Room.pb.h>

using namespace packet;

void PacketDispatcher::Initialize()
{
	// 초기화 코드 자동으로 생성됨
	RegisterPacket<ChatRes>(PacketID::ChatRes);
	RegisterPacket<JoinRoomRes>(PacketID::JoinRoomRes);
	RegisterPacket<LeaveRoomRes>(PacketID::LeaveRoomRes);
	RegisterPacket<ChatNot>(PacketID::ChatNot);
	RegisterPacket<JoinRoomNot>(PacketID::JoinRoomNot);
	RegisterPacket<LeaveRoomNot>(PacketID::LeaveRoomNot);
}