#pragma once
#include <memory>
#include <cstddef>
#include "Memory.h"

namespace hlib
{
	enum class BufferSize : size_t
	{
		Dynamic = 0,
		Tiny = 64,
		Small = 128,
		Medium = 256,
		Large = 512,
		XLarge = 1024,
		XXLarge = 2048,
		XXXLarge = 4096,
	};

	inline BufferSize GetEnumBufferSize(size_t size)
	{
		if (size <= 64) return BufferSize::Tiny;
		if (size <= 128) return BufferSize::Small;
		if (size <= 256) return BufferSize::Medium;
		if (size <= 512) return BufferSize::Large;
		if (size <= 1024) return BufferSize::XLarge;
		if (size <= 2048) return BufferSize::XXLarge;

		return BufferSize::Dynamic;
	}

	inline size_t GetByteSize(BufferSize type)
	{
		return static_cast<size_t>(type);
	}

	class Buffer
	{
	public:
		static std::byte* Acquire(size_t size)
		{
			return Memory::Instance()->GetAllocator<std::byte>().allocate(size);
		}

		static void Release(std::byte* ptr, size_t size)
		{
			if (ptr)
				Memory::Instance()->GetAllocator<std::byte>().deallocate(ptr, size);
		}
	};

}