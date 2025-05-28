#include "PacketHandler.h"
#include "PacketHandler.h"
#include <Log.h>
#include <Enums.pb.h>

#include "Client/ClientSession.h"
#include "PacketRegister.h"
#include "Room.h"
#include "User.h"
#include "PacketEnums.h"
#include "StringUtil.h"

using namespace packet;

PacketHandler::PacketHandler(Room& room) : room_(room)
{
}

void PacketHandler::OnDisconnected(std::shared_ptr<ClientSession>& session)
{
	room_.LeaveUser(session->GetSessionId());

	LeaveRoomNot leaveNot;
	leaveNot.set_sessionid(session->GetSessionId());

	room_.Broadcast(PacketRegister::Serialize(leaveNot), session->GetSessionId());
}

void PacketHandler::OnReceivePacket(std::shared_ptr<ClientSession>& session, const packet::JoinRoomReq& req)
{
	JoinRoomRes res;
	if (room_.IsMember(session->GetSessionId()))
	{
		auto errorCode = ErrorCode::FAIL_ALREADY_IN_ROOM;
		LOG_WARN("Session({}) : JoinRoomReq ErrorCode {}", session->GetSessionId(), errorCode);
		res.set_errorcode(errorCode);
		session->Send(PacketRegister::Serialize(res));
		return;
	}

	auto name = req.name();
	if (!RemoveWhitespace(name))
	{
		auto errorCode = ErrorCode::FAIL_EMPTY_NAME;
		LOG_WARN("Session({}) : JoinRoomReq ErrorCode {}", session->GetSessionId(), errorCode);
		res.set_errorcode(errorCode);
		session->Send(PacketRegister::Serialize(res));
		return;
	}

	if (Utf8Count(name) > Defaults::NAME_MAX_LIMIT)
	{
		auto errorCode = ErrorCode::FAIL_EXCEED_NAME_LIMIT;
		LOG_WARN("Session({}) : JoinRoomReq ErrorCode {}", session->GetSessionId(), errorCode);
		res.set_errorcode(errorCode);
		session->Send(PacketRegister::Serialize(res));
		return;
	}

	room_.JoinUser(session->GetSessionId(), name, session);
	room_.ForEachUser([&res](const User& user) {
		auto player = res.add_players();
		player->set_name(user.GetName());
		player->set_sessionid(user.GetSessionId());
	});

	LOG_DEBUG("Session({}) : Join Room", session->GetSessionId());
	res.set_errorcode(ErrorCode::OK);
	session->Send(PacketRegister::Serialize(res));

	JoinRoomNot joinNot;
	Player* joinPlayer = joinNot.mutable_player();
	joinPlayer->set_name(name);
	joinPlayer->set_sessionid(session->GetSessionId());

	room_.Broadcast(PacketRegister::Serialize(joinNot), session->GetSessionId());
}

void PacketHandler::OnReceivePacket(std::shared_ptr<ClientSession>& session, const packet::LeaveRoomReq& req)
{
	LeaveRoomRes res;
	if (!room_.IsMember(session->GetSessionId()))
	{
		auto errorCode = ErrorCode::FAIL_NOT_IN_ROOM;
		LOG_WARN("Session({}) : LeaveRoomReq ErrorCode {}", session->GetSessionId(), errorCode);
		res.set_errorcode(errorCode);
		session->Send(PacketRegister::Serialize(res));
		return;
	}

	room_.LeaveUser(session->GetSessionId());

	LOG_DEBUG("Session({}) : Leave Room", session->GetSessionId());
	res.set_errorcode(ErrorCode::OK);
	session->Send(PacketRegister::Serialize(res));

	LeaveRoomNot leaveNot;
	leaveNot.set_sessionid(session->GetSessionId());

	room_.Broadcast(PacketRegister::Serialize(leaveNot), session->GetSessionId());
}

void PacketHandler::OnReceivePacket(std::shared_ptr<ClientSession>& session, const packet::ChatReq& req)
{
	ChatRes res;
	if (!room_.IsMember(session->GetSessionId()))
	{
		auto errorCode = ErrorCode::FAIL_NOT_IN_ROOM;
		LOG_WARN("Session({}) : ChatReq ErrorCode {}", session->GetSessionId(), errorCode);
		res.set_errorcode(errorCode);
		session->Send(PacketRegister::Serialize(res));
		return;
	}

	auto desc = req.desc();
	if (!RemoveWhitespace(desc))
	{
		auto errorCode = ErrorCode::FAIL_EMPTY_MESSAGE;
		LOG_WARN("Session({}) : ChatReq ErrorCode {}", session->GetSessionId(), errorCode);
		res.set_errorcode(errorCode);
		session->Send(PacketRegister::Serialize(res));
		return;
	}

	if (desc.size() > Defaults::MESSAGE_MAX_LIMIT)
	{
		auto errorCode = ErrorCode::FAIL_EXCEED_MESSAGE_LIMIT;
		LOG_WARN("Session({}) : ChatReq ErrorCode {}", session->GetSessionId(), errorCode);
		res.set_errorcode(errorCode);
		session->Send(PacketRegister::Serialize(res));
		return;
	}

	//LOG_DEBUG("Session({}) : Chat ...", session->GetSessionId());
	//LOG_DEBUG("Session({}) : \"{}\"", session->GetSessionId(), desc);
	res.set_errorcode(ErrorCode::OK);
	res.set_desc(desc);
	session->Send(PacketRegister::Serialize(res));

	ChatNot chatNot;
	chatNot.set_sessionid(session->GetSessionId());
	chatNot.set_desc(desc);

	room_.Broadcast(PacketRegister::Serialize(chatNot), session->GetSessionId());
}
