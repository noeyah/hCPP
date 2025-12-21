#include "Wait.h"
#include "Server/Dummy.h"
#include "Log.h"

using namespace hlib;
using namespace packet;

namespace Task
{
	hlib::NodeStatus Wait::OnEnter()
	{
		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("Wait::OnEnter - dummy is null");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsConnected())
		{
			LOG_INFO("Wait::OnEnter - dummy is not connected");
			return NodeStatus::Failure;
		}

		// 눈팅중
		ResetEndTime();

		return NodeStatus::Success;
	}

	hlib::NodeStatus Wait::OnUpdate()
	{
		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("Wait::OnUpdate - dummy is null");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsConnected())
		{
			LOG_INFO("Wait::OnUpdate - dummy is not connected");
			return NodeStatus::Failure;
		}

		if (!IsExpired())
			return NodeStatus::Running;

		return NodeStatus::Success;
	}

	void Wait::OnExit()
	{
	}
}
