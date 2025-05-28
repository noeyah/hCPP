// 이 파일은 스크립트로 생성됨
// 위치 : Solution/Packet/jinja2/cpp_client

#pragma once
#include <memory>
#include <Task/Scheduler.h>

#include <Chat.pb.h>
#include <Enums.pb.h>
#include <Room.pb.h>

class ServerSession;
class Room;

class PacketHandler
{
public:
	PacketHandler(hlib::task::Scheduler& scheduler);

	void OnConnected(std::shared_ptr<ServerSession>& session);
	void OnDisconnected(std::shared_ptr<ServerSession>& session);

	// 자동 생성
	void OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::ChatRes& res);
	void OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::JoinRoomRes& res);
	void OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::LeaveRoomRes& res);

	void OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::ChatNot& noti);
	void OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::JoinRoomNot& noti);
	void OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::LeaveRoomNot& noti);
	// 여기까지 자동 생성

private:
	hlib::task::Scheduler& scheduler_;
};
