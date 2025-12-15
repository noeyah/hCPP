#pragma once
#include <memory>
#include <cstdint>
#include <cstring>
#include <array>

#include "WinCommon.h"
#include "Network/NetCommon.h"
#include "Network/InternalNetwork.h"
#include "Memory/Container.h"

namespace hlib
{
	class IoHandler;

	// 첫 멤버 or 상속으로, 메모리 시작 부분에 OVERLAPPED가 있어야 함. 가상 함수 쓰면 안됨.
	struct IoContext : public OVERLAPPED
	{
		IoContext(IoOperation io_op) : ioOperation(io_op)
		{
		}

		void Reset()
		{
			std::memset(this, 0, sizeof(OVERLAPPED));
			ioHandler.reset();
		}

		IoOperation ioOperation;
		std::shared_ptr<IoHandler> ioHandler;
	};

	struct AcceptContext : public IoContext
	{
		AcceptContext() : IoContext(IoOperation::Accept)
		{
		}

		void Reset()
		{
			IoContext::Reset();
			clientSocket = INVALID_SOCKET;
			clientAddressBuffer.fill(0);
		}

		SOCKET clientSocket = INVALID_SOCKET;
		std::array<char, (sizeof(SOCKADDR_IN) + SOCKADDR_PADDING) * 2> clientAddressBuffer{};
	};

	struct SendContext : public IoContext
	{
		SendContext() : IoContext(IoOperation::Send)
		{
		}

		void Reset()
		{
			IoContext::Reset();
			sendSize = 0;
			wsaBufs.clear();
		}

		size_t sendSize = 0;
		Vector<WSABUF> wsaBufs;
	};
}