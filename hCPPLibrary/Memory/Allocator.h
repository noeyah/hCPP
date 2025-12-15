#pragma once
#include <memory>
#include <iostream>
#include "Util/Macro.h"
#include "Memory.h"

namespace hlib
{
	template<typename T, typename... Args>
	std::shared_ptr<T> MakeSharedPtr(Args&&... args)
	{
		auto allocator = Memory::Instance()->GetAllocator<T>();

		return std::allocate_shared<T>(allocator, std::forward<Args>(args)...);
	}

}