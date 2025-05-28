#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "WinCommon.h"

namespace hlib::net
{
#pragma pack(push, 1)
	struct PacketHeader
	{
		uint16_t size;
		uint16_t id;
	};
#pragma pack(pop)

	using SessionId = uint64_t;

	// ���� �ִ� ũ��
	inline constexpr size_t MAX_BUFFER_SIZE = 4096;

	inline constexpr size_t HEADER_SIZE = sizeof(PacketHeader);
}