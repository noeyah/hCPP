#pragma once
#include <cstdint>
#include "WinCommon.h"

namespace hlib::net
{
	// ���ο� ����

	enum class IoOperation : uint8_t
	{
		None,
		Accept,
		Connect,
		Disconnect,
		Recv,
		Send,
	};

	// winsock���� �ּ� ���� ������ ����ü ũ�� + 16����Ʈ �ؾ���.
	inline constexpr DWORD SOCKADDR_PADDING = 16;

}