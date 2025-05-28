#pragma once

namespace hlib
{
	enum class MemoryMode
	{
		None,
		Pool,
		Stomp,
		StompSlim,
		Log,
	};

	inline constexpr MemoryMode MEMORY_RESOURCE_MODE = MemoryMode::Pool;
	inline constexpr size_t MEMORY_POOL_MAX_BLOCKS_PER_CHUNK = 0;
	inline constexpr size_t MEMORY_POOL_LARGEST_BLOCK = 4096;
}