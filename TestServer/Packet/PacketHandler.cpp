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
#include "Helper.h"

using namespace packet;

void PacketHandler::OnDisconnected(SessionPtr pSession)
{
	auto pRoom = Helper::GetRoom();
	if (!pRoom)
	{
		LOG_ERROR("room is null");
		return;
	}

	pRoom->LeaveUser(pSession->GetID());

	LeaveRoomNot leaveNot;
	leaveNot.set_sessionid(pSession->GetID());

	pRoom->Broadcast(PacketRegister::Serialize(leaveNot), pSession->GetID());
}

void PacketHandler::OnReceivePacket(SessionPtr pSession, const packet::ChatReq& req)
{
	auto pRoom = Helper::GetRoom();
	if (!pRoom)
	{
		LOG_ERROR("room is null");
		return;
	}

	ChatRes res;
	if (!pRoom->IsMember(pSession->GetID()))
	{
		auto errorCode = ErrorCode::FAIL_NOT_IN_ROOM;
		LOG_WARN("Session({}) : ChatReq ErrorCode {}", pSession->GetID(), errorCode);
		res.set_errorcode(errorCode);
		pSession->Send(PacketRegister::Serialize(res));
		return;
	}

	auto desc = req.desc();
	if (!RemoveWhitespace(desc))
	{
		auto errorCode = ErrorCode::FAIL_EMPTY_MESSAGE;
		LOG_WARN("Session({}) : ChatReq ErrorCode {}", pSession->GetID(), errorCode);
		res.set_errorcode(errorCode);
		pSession->Send(PacketRegister::Serialize(res));
		return;
	}

	if (desc.size() > Defaults::MESSAGE_MAX_LIMIT)
	{
		auto errorCode = ErrorCode::FAIL_EXCEED_MESSAGE_LIMIT;
		LOG_WARN("Session({}) : ChatReq ErrorCode {}", pSession->GetID(), errorCode);
		res.set_errorcode(errorCode);
		pSession->Send(PacketRegister::Serialize(res));
		return;
	}

	//LOG_DEBUG("Session({}) : Chat ...", pSession->GetID());
	//LOG_DEBUG("Session({}) : \"{}\"", pSession->GetSessionId(), desc);
	res.set_errorcode(ErrorCode::OK);
	res.set_desc(desc);
	pSession->Send(PacketRegister::Serialize(res));

	ChatNot chatNot;
	chatNot.set_sessionid(pSession->GetID());
	chatNot.set_desc(desc);

	pRoom->Broadcast(PacketRegister::Serialize(chatNot), pSession->GetID());
}

void PacketHandler::OnReceivePacket(SessionPtr pSession, const packet::JoinRoomReq& req)
{
	auto pRoom = Helper::GetRoom();
	if (!pRoom)
	{
		LOG_ERROR("room is null");
		return;
	}

	JoinRoomRes res;
	if (pRoom->IsMember(pSession->GetID()))
	{
		auto errorCode = ErrorCode::FAIL_ALREADY_IN_ROOM;
		LOG_WARN("Session({}) : JoinRoomReq ErrorCode {}", pSession->GetID(), errorCode);
		res.set_errorcode(errorCode);
		pSession->Send(PacketRegister::Serialize(res));
		return;
	}

	auto name = req.name();
	if (!RemoveWhitespace(name))
	{
		auto errorCode = ErrorCode::FAIL_EMPTY_NAME;
		LOG_WARN("Session({}) : JoinRoomReq ErrorCode {}", pSession->GetID(), errorCode);
		res.set_errorcode(errorCode);
		pSession->Send(PacketRegister::Serialize(res));
		return;
	}

	if (Utf8Count(name) > Defaults::NAME_MAX_LIMIT)
	{
		auto errorCode = ErrorCode::FAIL_EXCEED_NAME_LIMIT;
		LOG_WARN("Session({}) : JoinRoomReq ErrorCode {}", pSession->GetID(), errorCode);
		res.set_errorcode(errorCode);
		pSession->Send(PacketRegister::Serialize(res));
		return;
	}

	pRoom->JoinUser(pSession->GetID(), name, pSession);
	pRoom->ForEachUser([&res](const User& user) {
		auto player = res.add_players();
		player->set_name(user.GetName());
		player->set_sessionid(user.GetSessionId());
	});

	LOG_DEBUG("Session({}) : Join Room", pSession->GetID());
	res.set_errorcode(ErrorCode::OK);
	pSession->Send(PacketRegister::Serialize(res));

	JoinRoomNot joinNot;
	Player* joinPlayer = joinNot.mutable_player();
	joinPlayer->set_name(name);
	joinPlayer->set_sessionid(pSession->GetID());

	pRoom->Broadcast(PacketRegister::Serialize(joinNot), pSession->GetID());

}

void PacketHandler::OnReceivePacket(SessionPtr pSession, const packet::LeaveRoomReq& req)
{
	auto pRoom = Helper::GetRoom();
	if (!pRoom)
	{
		LOG_ERROR("room is null");
		return;
	}

	LeaveRoomRes res;
	if (!pRoom->IsMember(pSession->GetID()))
	{
		auto errorCode = ErrorCode::FAIL_NOT_IN_ROOM;
		LOG_WARN("Session({}) : LeaveRoomReq ErrorCode {}", pSession->GetID(), errorCode);
		res.set_errorcode(errorCode);
		pSession->Send(PacketRegister::Serialize(res));
		return;
	}

	pRoom->LeaveUser(pSession->GetID());

	LOG_DEBUG("Session({}) : Leave Room", pSession->GetID());
	res.set_errorcode(ErrorCode::OK);
	pSession->Send(PacketRegister::Serialize(res));

	LeaveRoomNot leaveNot;
	leaveNot.set_sessionid(pSession->GetID());

	pRoom->Broadcast(PacketRegister::Serialize(leaveNot), pSession->GetID());
}
