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
		auto allocator = Memory::Instance().GetAllocator<T>();

		return std::allocate_shared<T>(allocator, std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	T* Alloc(Args&&... args)
	{
		void* rawMemory = Memory::Instance().Allocate(sizeof(T), alignof(T));
		
		T* ptr;
		try
		{
			ptr = static_cast<T*>(rawMemory);
			new (ptr)T(std::forward<Args>(args)...);
		}
		catch (...)
		{
			Memory::Instance().Release(rawMemory, sizeof(T), alignof(T));
			CRASH("faild Alloc");
		}

		return ptr;
	}

	template<typename T>
	void Delete(T* ptr) noexcept
	{
		if (!ptr)
			return;

		ptr->~T();
		Memory::Instance().Release(ptr, sizeof(T), alignof(T));
	}

	template <typename T>
	struct Deleter
	{
		void operator()(T* ptr) const
		{
			Delete(ptr);
		}
	};

	template<typename T, typename... Args>
	std::unique_ptr<T, Deleter<T>> MakeUniquePtr(Args&&... args)
	{
		return std::unique_ptr<T, Deleter<T>>(Alloc<T>(std::forward<Args>(args)...));
	}
}