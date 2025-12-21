#pragma once
#include "Base.h"

namespace Task
{
	class Connect : public Base
	{
	public:
		using Base::Base;

		hlib::NodeStatus OnEnter() override;
		hlib::NodeStatus OnUpdate() override;
		void OnExit() override;
	};
}
