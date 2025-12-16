#pragma once
#include <memory>
#include <functional>
#include <cstdint>

namespace Helper
{
	void PushSchedule(uint64_t ms, std::function<void()> fn);
}
