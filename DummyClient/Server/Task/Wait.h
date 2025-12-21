#pragma once
#include "Base.h"
#include "Util/TimeUtil.h"

namespace Task
{
	class Wait : public Base
	{
		hlib::Time::steady_time m_endTime{};
		uint64_t m_waitTime{};

	public:
		Wait(std::weak_ptr<Dummy> wpDummy, uint64_t ms) : Base(wpDummy), m_waitTime(ms) {}

		hlib::NodeStatus OnEnter() override;
		hlib::NodeStatus OnUpdate() override;
		void OnExit() override;

		void ResetEndTime()
		{
			m_endTime = hlib::Time::TimePointAfterFromNow(m_waitTime);
		}

		bool IsExpired()
		{
			return hlib::Time::IsExpired(m_endTime);
		}
	};
}
