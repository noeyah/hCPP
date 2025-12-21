#include "PacketHandler.h"
#include <string>
#include <format>
#include <Log.h>
#include <Enums.pb.h>
#include "Server/ServerSession.h"
#include "PacketEnums.h"
#include "Server/Dummy.h"

using namespace packet;


void PacketHandler::OnConnected(SessionPtr pSession)
{
	auto pDummy = pSession->GetDummy();
	if (!pDummy)
		return;

	pDummy->SetState(DummyState::Connected);
	LOG_INFO("Dummy {} Connected", pDummy->GetID());
}

void PacketHandler::OnDisconnected(SessionPtr pSession)
{
	auto pDummy = pSession->GetDummy();
	if (!pDummy)
	{
		LOG_ERROR("dummy is null ({})", pSession->GetID());
		return;
	}

	pDummy->SetState(DummyState::Disconnected);
}

void PacketHandler::OnReceivePacket(SessionPtr pSession, const packet::ChatRes& res)
{
	auto pDummy = pSession->GetDummy();
	if (!pDummy)
	{
		LOG_ERROR("dummy is null ({})", pSession->GetID());
		return;
	}

	if (res.errorcode() != ErrorCode::OK)
	{
		LOG_ERROR("dummy {}({}) : ChatRes errorCode {}", pDummy->GetID(), pSession->GetID(), res.errorcode());
		return;
	}

}

void PacketHandler::OnReceivePacket(SessionPtr pSession, const packet::JoinRoomRes& res)
{
	auto pDummy = pSession->GetDummy();
	if (!pDummy)
	{
		LOG_ERROR("dummy is null ({})", pSession->GetID());
		return;
	}

	if (res.errorcode() != ErrorCode::OK)
	{
		LOG_ERROR("dummy {}({}) : JoinRoomRes errorCode {}", pDummy->GetID(), pSession->GetID(), res.errorcode());
		return;
	}

	pDummy->SetRoom(true);
}

void PacketHandler::OnReceivePacket(SessionPtr pSession, const packet::LeaveRoomRes& res)
{
	auto pDummy = pSession->GetDummy();
	if (!pDummy)
	{
		LOG_ERROR("dummy is null ({})", pSession->GetID());
		return;
	}

	if (res.errorcode() != ErrorCode::OK)
	{
		LOG_ERROR("dummy {}({}) : LeaveRoomRes errorCode {}", pDummy->GetID(), pSession->GetID(), res.errorcode());
		return;
	}

	pDummy->SetRoom(false);
}

void PacketHandler::OnReceivePacket(SessionPtr pSession, const packet::ChatNot& noti)
{
}

void PacketHandler::OnReceivePacket(SessionPtr pSession, const packet::JoinRoomNot& noti)
{
}

void PacketHandler::OnReceivePacket(SessionPtr pSession, const packet::LeaveRoomNot& noti)
{
}

