// 이 파일은 스크립트로 생성됨
// 위치 : Solution/Packet/jinja2/cpp

#pragma once
#include <memory>

#include <Chat.pb.h>
#include <Enums.pb.h>
#include <Room.pb.h>

class ClientSession;
class Room;

class PacketHandler
{
public:
	PacketHandler(Room& room);
	void OnDisconnected(std::shared_ptr<ClientSession>& session);

	// 자동 생성
	void OnReceivePacket(std::shared_ptr<ClientSession>& session, const packet::ChatReq& req);
	void OnReceivePacket(std::shared_ptr<ClientSession>& session, const packet::JoinRoomReq& req);
	void OnReceivePacket(std::shared_ptr<ClientSession>& session, const packet::LeaveRoomReq& req);
	// 여기까지 자동 생성

private:
	Room& room_;
};