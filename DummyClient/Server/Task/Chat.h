#pragma once
#include "Base.h"

namespace Task
{
	class Chat : public Base
	{
	public:
		using Base::Base;

		hlib::NodeStatus OnEnter() override;
		hlib::NodeStatus OnUpdate() override;
		void OnExit() override;
	};
}
