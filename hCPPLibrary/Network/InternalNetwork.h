#pragma once
#include <cstdint>
#include "WinCommon.h"

namespace hlib::net
{
	// 내부용 정의

	enum class IoOperation : uint8_t
	{
		None,
		Accept,
		Connect,
		Disconnect,
		Recv,
		Send,
	};

	// winsock에서 주소 저장 공간을 구조체 크기 + 16바이트 해야함.
	inline constexpr DWORD SOCKADDR_PADDING = 16;

}