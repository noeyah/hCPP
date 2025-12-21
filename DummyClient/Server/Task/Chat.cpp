#include "Chat.h"
#include <format>
#include <string>
#include "Chat.pb.h"
#include "Server/Dummy.h"
#include "Util/RandomUtil.h"
#include "Log.h"

using namespace hlib;
using namespace packet;

namespace Task
{
	hlib::NodeStatus Chat::OnEnter()
	{
		static const std::vector<std::wstring> description = {
			L"안녕하세요! 저는 더미에오!",
			L"커피 수혈하실래요?",
			L"일이삼사오육칠팔구십일이삼사오육칠팔구십일이삼사오육칠팔구십",
			L"하루에 물을 2L 마시면 좋대요",
			L"더미가 말을 한다 덤덤덤",
			L"월요일 좋아~ 최고로 좋아~\n난 일할 때 젤 멋지지~\n오늘부터 열심히 할거야~\n오 좋아~ 월요일 좋아~\n같이 불러 핑핑아!",
			L"냐냐냐냐냐 냐냐냐냐냐\n냐냐냐냐냐냐냐냐"
		};

		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("Chat::OnEnter - dummy is null");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsConnected())
		{
			LOG_INFO("Chat::OnEnter - dummy is not connected");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsInRoom())
		{
			LOG_INFO("Chat::OnEnter - not in room");
			return NodeStatus::Failure;
		}

		int idx = Random::RandomRange(0, static_cast<int>(description.size()) - 1);
		std::string msg = hlib::ConvertToUtf8(description[idx]);

		ChatReq req;
		req.set_desc(msg);

		if (!pDummy->Send(req))
		{
			LOG_INFO("Chat::OnEnter - dummy send fail");
			return NodeStatus::Failure;
		}

		return NodeStatus::Success;
	}

	hlib::NodeStatus Chat::OnUpdate()
	{
		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("Chat::OnUpdate - dummy is null");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsConnected())
		{
			LOG_INFO("Chat::OnUpdate - dummy is not connected");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsInRoom())
		{
			LOG_INFO("Chat::OnUpdate - dummy is not room");
			return NodeStatus::Failure;
		}

		return NodeStatus::Success;
	}

	void Chat::OnExit()
	{
	}
}
