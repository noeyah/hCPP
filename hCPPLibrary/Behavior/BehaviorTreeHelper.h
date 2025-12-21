#pragma once
#include <memory>
#include <utility>
#include "BehaviorTree.h"

namespace hlib
{
	template<bt::Task T, typename... Args>
	std::shared_ptr<IBTNode> Action(Args&&... args)
	{
		return std::make_shared<bt::Action<T>>(std::forward<Args>(args)...);
	}

	template<typename... Nodes>
	std::shared_ptr<IBTNode> Sequence(Nodes&&... nodes)
	{
		return std::make_shared<bt::Sequence>(std::forward<Nodes>(nodes)...);
	}

	template<typename... Nodes>
	std::shared_ptr<IBTNode> Selector(Nodes&&... nodes)
	{
		return std::make_shared<bt::Selector>(std::forward<Nodes>(nodes)...);
	}

	template<typename... Nodes>
	std::shared_ptr<IBTNode> RandomSelector(Nodes&&... nodes)
	{
		return std::make_shared<bt::RandomSelector>(std::forward<Nodes>(nodes)...);
	}

	template<typename Func>
	std::shared_ptr<IBTNode> Condition(Func&& cond, std::shared_ptr<IBTNode> child)
	{
		return std::make_shared<bt::Condition>(std::forward<Func>(cond), child);
	}

	template<typename Node>
	std::shared_ptr<BehaviorTree> CreateBT(Node&& node)
	{
		return std::make_shared<BehaviorTree>(std::forward<Node>(node));
	}
}