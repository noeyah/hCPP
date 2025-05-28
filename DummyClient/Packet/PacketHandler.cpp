#include "PacketHandler.h"
#include <string>
#include <format>
#include <Log.h>
#include <Enums.pb.h>
#include <Task/TaskUtil.h>
#include <Util/RandomUtil.h>
#include <Util/StringUtil.h>
#include "Server/ServerSession.h"
#include "PacketRegister.h"
#include "PacketEnums.h"

using namespace packet;

PacketHandler::PacketHandler(hlib::task::Scheduler& scheduler) : scheduler_(scheduler)
{
}

void PacketHandler::OnConnected(std::shared_ptr<ServerSession>& session)
{
	const std::vector<hlib::Random::WeightItem<std::wstring>> nameItems = {
		{ L"김더미", 21 }, { L"이더미", 14 }, { L"박더미", 8 }, { L"최더미", 4 }, { L"정더미", 4 },
		{ L"강더미", 2 }, { L"조더미", 2 }, { L"윤더미", 2 }, { L"장더미", 1 }, { L"임더미", 1 }
	};

	std::wstring name = hlib::Random::GetRandomWeight(nameItems);

	scheduler_.PushAfter(1000, [session, name]() {
		JoinRoomReq req;
		req.set_name(hlib::ConvertToUtf8(name));
		session->Send(PacketRegister::Serialize(req));
		session->SetSendTime();
	});
}

void PacketHandler::OnDisconnected(std::shared_ptr<ServerSession>& session)
{
}

void PacketHandler::OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::ChatRes& res)
{
	if (res.errorcode() != ErrorCode::OK)
	{
		LOG_ERROR("dummy({}) : ChatRes errorCode {}", session->GetID(), res.errorcode());
		return;
	}
	
	const std::vector<hlib::Random::WeightItem<std::wstring>> chatItems = {
		{ L"테스트 엔터\n\n엔터 두번했어요\n중간에 빈 한줄이 보이나요?", 20 },
		{ L"날\n씨\n짱\n좋\n앙", 10 },
		{ L"공              백           많           이             주           세          요ㅎ", 10 },
		{ L"가로로 길~게 말해보아요. 옆으로 쭈욱쭈욱 늘려보아요. 이거 최대 길이가 500자였나. 힣", 20 },
		{ L"물을 많이 마시면 좋대요.", 20 },
		{ L"나는 백수백수~ 직업이 없네~", 20 }
	};

	std::wstring desc = hlib::Random::GetRandomWeight(chatItems);
	scheduler_.PushAfter(hlib::Random::RandomRange(500, 2000), [session, desc]() {
		ChatReq req;
		req.set_desc(hlib::ConvertToUtf8(desc));
		session->Send(PacketRegister::Serialize(req));
		session->SetSendTime();
	});

	scheduler_.PushAfter(5000, [session]() {
		if (session->IsSendTimeout())
		{
			LOG_ERROR("TIMEOUT ChatReq : {}", session->GetID());
			session->TimeOff();
		}
	});
}

void PacketHandler::OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::JoinRoomRes& res)
{
	if (res.errorcode() != ErrorCode::OK)
	{
		LOG_ERROR("dummy({}) : JoinRoomRes errorCode {}", session->GetID(), res.errorcode());
		return;
	}

	ChatReq req;
	req.set_desc(hlib::ConvertToUtf8(L"더 미 등 장"));
	session->Send(PacketRegister::Serialize(req));
	session->SetSendTime();
}

void PacketHandler::OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::LeaveRoomRes& res)
{
	if (res.errorcode() != ErrorCode::OK)
	{
		LOG_ERROR("dummy({}) : LeaveRoomRes errorCode {}", session->GetID(), res.errorcode());
		return;
	}

	scheduler_.PushAfter(5000, [session]() {
		JoinRoomReq req;
		req.set_name(hlib::ConvertToUtf8(L"돌아온 더미쟝"));
		session->Send(PacketRegister::Serialize(req));
	});
}

void PacketHandler::OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::ChatNot& noti)
{
}

void PacketHandler::OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::JoinRoomNot& noti)
{
}

void PacketHandler::OnReceivePacket(std::shared_ptr<ServerSession>& session, const packet::LeaveRoomNot& noti)
{
}
