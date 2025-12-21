#include "JoinRoom.h"
#include <format>
#include "Room.pb.h"
#include "Server/Dummy.h"
#include "Log.h"

using namespace hlib;
using namespace packet;

namespace Task
{
	hlib::NodeStatus JoinRoom::OnEnter()
	{
		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("JoinRoom::OnEnter - dummy is null");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsConnected())
		{
			LOG_INFO("JoinRoom::OnEnter - dummy is not connected");
			return NodeStatus::Failure;
		}

		if (pDummy->IsInRoom())
		{
			LOG_INFO("JoinRoom::OnEnter - already in room");
			return NodeStatus::Failure;
		}

		JoinRoomReq req;
		std::string name = std::format("dummy {:03d}", pDummy->GetID());
		req.set_name(name);

		if (!pDummy->Send(req))
		{
			LOG_INFO("JoinRoom::OnEnter - dummy send fail");
			return NodeStatus::Failure;
		}

		return NodeStatus::Success;
	}

	hlib::NodeStatus JoinRoom::OnUpdate()
	{
		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("JoinRoom::OnUpdate - dummy is null");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsConnected())
		{
			LOG_INFO("JoinRoom::OnUpdate - dummy is not connected");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsInRoom())
		{
			return NodeStatus::Running;
		}

		return NodeStatus::Success;
	}

	void JoinRoom::OnExit()
	{
	}
}
