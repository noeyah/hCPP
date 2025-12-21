#include "Think.h"
#include <vector>
#include "Server/Dummy.h"
#include "Log.h"
#include "Util/RandomUtil.h"

using namespace hlib;

namespace Task
{

	hlib::NodeStatus Think::OnEnter()
	{
		auto pDummy = GetDummy();
		if (!pDummy)
		{
			LOG_INFO("Think::OnEnter - dummy is null");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsConnected())
		{
			LOG_INFO("Think::OnEnter - dummy is not connected");
			return NodeStatus::Failure;
		}

		if (!pDummy->IsInRoom())
		{
			LOG_INFO("Think::OnEnter - not in room");
			return NodeStatus::Failure;
		}

		const std::vector < Random::WeightItem<DummyTaskInRoom>> tasks = {
			{ DummyTaskInRoom::Chat, 50 },
			{ DummyTaskInRoom::LeaveRoom, 10 },
			{ DummyTaskInRoom::Wait, 40 }
		};

		auto nextTask = Random::GetRandomWeight(tasks);
		pDummy->SetNextTask(nextTask);

		return hlib::NodeStatus::Success;
	}

	hlib::NodeStatus Think::OnUpdate()
	{
		return hlib::NodeStatus::Success;
	}

	void Think::OnExit()
	{
	}
}
