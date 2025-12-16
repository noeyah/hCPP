#pragma once
#include <memory>
#include <Task/Scheduler.h>

#include <Chat.pb.h>
#include <Enums.pb.h>
#include <Room.pb.h>

class ServerSession;
using SessionPtr = std::shared_ptr<ServerSession>;

class PacketHandler
{
public:
	PacketHandler() = delete;

	static void OnConnected(SessionPtr pSession);
	static void OnDisconnected(SessionPtr pSession);

	static void OnReceivePacket(SessionPtr pSession, const packet::ChatRes& res);
	static void OnReceivePacket(SessionPtr pSession, const packet::JoinRoomRes& res);
	static void OnReceivePacket(SessionPtr pSession, const packet::LeaveRoomRes& res);

	static void OnReceivePacket(SessionPtr pSession, const packet::ChatNot& noti);
	static void OnReceivePacket(SessionPtr pSession, const packet::JoinRoomNot& noti);
	static void OnReceivePacket(SessionPtr pSession, const packet::LeaveRoomNot& noti);
};
