#pragma once
#include <memory>
#include <cstddef>
#include "Memory.h"

namespace hlib
{
	class Buffer
	{
	public:
		static std::shared_ptr<std::byte[]> Get(size_t bufferSize)
		{
			auto allocator = Memory::Instance()->GetAllocator<std::byte>();

			std::byte* buffer = allocator.allocate(bufferSize);

			auto deleter = [alloc = allocator, size = bufferSize](std::byte* ptr) mutable {
				if (ptr)
				{
					alloc.deallocate(ptr, size);
				}
			};

			return std::shared_ptr<std::byte[]>(buffer, std::move(deleter));
		}

	};

}