#pragma once
#include <memory>

#include <Chat.pb.h>
#include <Enums.pb.h>
#include <Room.pb.h>

class ClientSession;
class Room;
using SessionPtr = std::shared_ptr<ClientSession>;

class PacketHandler
{
public:
	PacketHandler() = delete;

	static void OnDisconnected(SessionPtr pSession);

	static void OnReceivePacket(SessionPtr pSession, const packet::ChatReq& req);
	static void OnReceivePacket(SessionPtr pSession, const packet::JoinRoomReq& req);
	static void OnReceivePacket(SessionPtr pSession, const packet::LeaveRoomReq& req);
};