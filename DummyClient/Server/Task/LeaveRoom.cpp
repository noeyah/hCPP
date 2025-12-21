#include "LeaveRoom.h"
#include "Room.pb.h"
#include "Server/Dummy.h"
#include "Log.h"

using namespace hlib;
using namespace packet;

namespace Task
{
	hlib::NodeStatus LeaveRoom::OnEnter()
	{
		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("LeaveRoom::OnEnter - dummy is null");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsConnected())
		{
			LOG_INFO("LeaveRoom::OnEnter - dummy is not connected");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsInRoom())
		{
			LOG_INFO("LeaveRoom::OnEnter - not in room");
			return NodeStatus::Failure;
		}

		LeaveRoomReq req;
		if (!pDummy->Send(req))
		{
			LOG_INFO("LeaveRoom::OnEnter - dummy send fail");
			return NodeStatus::Failure;
		}

		return NodeStatus::Success;
	}

	hlib::NodeStatus LeaveRoom::OnUpdate()
	{
		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("LeaveRoom::OnUpdate - dummy is null");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsConnected())
		{
			LOG_INFO("LeaveRoom::OnUpdate - dummy is not connected");
			return NodeStatus::Failure;
		}

		if (pDummy->IsInRoom())
		{
			return NodeStatus::Running;
		}

		return NodeStatus::Success;
	}

	void LeaveRoom::OnExit()
	{
	}
}
