#pragma once
#include <memory>
#include "bt.h"

class Dummy;

namespace Task
{
	class Base : public hlib::IBTTask
	{
	protected:
		std::weak_ptr<Dummy> m_wpDummy{};

	public:
		Base(std::weak_ptr<Dummy> wpDummy) : m_wpDummy(wpDummy) {}
		virtual ~Base() = default;

	protected:
		std::shared_ptr<Dummy> GetDummy() { return m_wpDummy.lock(); }
	};
}
