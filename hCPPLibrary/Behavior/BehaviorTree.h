#pragma once
#include <concepts>
#include <vector>
#include <memory>
#include <functional>
#include "Util/RandomUtil.h"

namespace hlib
{
	enum class NodeStatus
	{
		Success,
		Failure,
		Running,

		Invalid
	};

	class IBTNode
	{
	public:
		virtual ~IBTNode() = default;
		virtual NodeStatus Tick() = 0;
		virtual void Halt() {}
	};

	class IBTTask
	{
	public:
		virtual ~IBTTask() = default;
		virtual NodeStatus OnEnter() = 0;
		virtual NodeStatus OnUpdate() = 0;
		virtual void OnExit() = 0;
	};

	class BehaviorTree
	{
		std::shared_ptr<IBTNode> m_root;

	public:
		BehaviorTree() = default;
		BehaviorTree(std::shared_ptr<IBTNode> root) : m_root(root) {}

		void SetRoot(std::shared_ptr<IBTNode> root)
		{
			m_root = root;
		}

		void Tick()
		{
			if (!m_root)
				return;

			m_root->Tick();
		}
	};

	namespace bt
	{
		template<typename T>
		concept Node = std::derived_from<T, IBTNode>;

		template<typename T>
		concept Task = std::derived_from<T, IBTTask>;

		class Composite : public IBTNode
		{
		protected:
			std::vector<std::shared_ptr<IBTNode>> m_children{};
			size_t m_currentIndex{};
			NodeStatus m_eStatus{ NodeStatus::Invalid };

		public:
			template<Node... Args>
			Composite(std::shared_ptr<Args>... nodes)
			{
				(m_children.emplace_back(nodes), ...);
			}

			void AddChild(std::shared_ptr<IBTNode> node)
			{
				m_children.emplace_back(node);
			}

			void Halt() override
			{
				if (m_currentIndex < m_children.size())
					m_children[m_currentIndex]->Halt();

				m_eStatus = NodeStatus::Invalid;
				m_currentIndex = 0;
			}
		};

		class Decorator : public IBTNode
		{
		protected:
			std::shared_ptr<IBTNode> m_child{};
			NodeStatus m_eStatus{ NodeStatus::Invalid };

		public:
			Decorator(std::shared_ptr<IBTNode> child) : m_child(child) {}

			void Halt() override
			{
				if (m_child)
					m_child->Halt();

				m_eStatus = NodeStatus::Invalid;
			}
		};

		class Sequence : public Composite
		{
		public:
			using Composite::Composite;

			NodeStatus Tick() override
			{
				if (m_eStatus != NodeStatus::Running)
				{
					m_currentIndex = 0;
				}

				while (m_currentIndex < m_children.size())
				{
					auto status = m_children[m_currentIndex]->Tick();

					if (status == NodeStatus::Running)
					{
						m_eStatus = status;
						return m_eStatus;
					}

					if (status == NodeStatus::Failure)
					{
						m_eStatus = status;
						return m_eStatus;
					}

					m_currentIndex++;
				}

				m_currentIndex = 0;
				m_eStatus = NodeStatus::Success;
				return m_eStatus;
			}

		};

		class Selector : public Composite
		{
		public:
			using Composite::Composite;

			NodeStatus Tick() override
			{
				if (m_eStatus != NodeStatus::Running)
				{
					m_currentIndex = 0;
				}

				while (m_currentIndex < m_children.size())
				{
					auto status = m_children[m_currentIndex]->Tick();

					if (status == NodeStatus::Running)
					{
						m_eStatus = status;
						return m_eStatus;
					}

					if (status == NodeStatus::Success)
					{
						m_currentIndex = 0;
						m_eStatus = status;
						return m_eStatus;
					}

					m_currentIndex++;
				}

				m_currentIndex = 0;
				m_eStatus = NodeStatus::Failure;
				return m_eStatus;
			}
		};


		class RandomSelector : public Composite
		{
		public:
			using Composite::Composite;

			NodeStatus Tick() override
			{
				if (m_children.empty())
					return NodeStatus::Failure;

				if (m_eStatus != NodeStatus::Running)
				{
					m_currentIndex = Random::RandomRange(0, static_cast<int>(m_children.size()) - 1);
				}

				m_eStatus = m_children[m_currentIndex]->Tick();

				return m_eStatus;
			}
		};

		class Condition : public Decorator
		{
			std::function<bool()> m_cond{};

		public:
			Condition(std::function<bool()> cond, std::shared_ptr<IBTNode> child)
				: Decorator(child), m_cond(cond) {
			}

			NodeStatus Tick() override
			{
				if (!m_child)
					return NodeStatus::Failure;

				if (m_eStatus != NodeStatus::Running)
				{
					if (!m_cond())
					{
						m_eStatus = NodeStatus::Failure;
						return m_eStatus;
					}
				}

				m_eStatus = m_child->Tick();
				return m_eStatus;
			}
		};

		template<Task T>
		class Action : public IBTNode
		{
			T m_logic;
			NodeStatus m_eStatus{ NodeStatus::Invalid };

		public:
			template<typename... Args>
			Action(Args&&... args) : m_logic(std::forward<Args>(args)...) {}

			NodeStatus Tick() override
			{
				if (m_eStatus == NodeStatus::Invalid)
				{
					m_eStatus = m_logic.OnEnter();
				}

				if (m_eStatus == NodeStatus::Success
					|| m_eStatus == NodeStatus::Running)
				{
					m_eStatus = m_logic.OnUpdate();
				}

				if (m_eStatus != NodeStatus::Running)
				{
					m_logic.OnExit();
					auto result = m_eStatus;
					m_eStatus = NodeStatus::Invalid;
					return result;
				}

				return m_eStatus;
			}

			void Halt() override
			{
				if (m_eStatus == NodeStatus::Running)
					m_logic.OnExit();

				m_eStatus = NodeStatus::Invalid;
			}
		};
	}
}