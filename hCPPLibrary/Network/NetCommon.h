#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "WinCommon.h"

namespace hlib::net
{
	struct PacketHeader
	{
		uint16_t size;
		uint16_t id;
	};

	using SessionId = uint64_t;

	// 버퍼 최대 크기
	inline constexpr size_t MAX_BUFFER_SIZE = 4096;

	inline constexpr size_t HEADER_SIZE = sizeof(PacketHeader);
}