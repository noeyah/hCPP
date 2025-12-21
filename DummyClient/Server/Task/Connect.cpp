#include "Connect.h"
#include "Server/Dummy.h"
#include "Log.h"

using namespace hlib;

namespace Task
{
	hlib::NodeStatus Connect::OnEnter()
	{
		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("Connect::OnEnter - dummy is null");
			return NodeStatus::Failure;
		}

		// 연결 되어있으면 리턴
		if (pDummy->IsConnected())
		{
			LOG_INFO("Connect::OnEnter - dummy is connected");
			return NodeStatus::Failure;
		}

		// 연결 요청
		if (!pDummy->Connect())
		{
			LOG_INFO("Connect::OnEnter - connect fail");
			return NodeStatus::Failure;
		}

		return NodeStatus::Success;
	}

	hlib::NodeStatus Connect::OnUpdate()
	{
		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("Connect::OnUpdate - dummy is null");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsConnected())
			return NodeStatus::Running;

		return NodeStatus::Success;
	}

	void Connect::OnExit()
	{
	}
}
