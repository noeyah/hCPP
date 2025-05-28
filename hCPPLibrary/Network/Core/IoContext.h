#pragma once
#include <memory>
#include <cstdint>
#include <cstring>
#include <array>

#include "WinCommon.h"
#include "Network/NetCommon.h"
#include "Network/InternalNetwork.h"
#include "Memory/Container.h"
#include <string>
#include <atomic>

namespace hlib::net
{
	class IIoCompletionHandler;

	// ù ��� or �������, �޸� ���� �κп� OVERLAPPED�� �־�� ��. ���� �Լ� ���� �ȵ�.
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
		std::shared_ptr<IIoCompletionHandler> ioHandler;
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

	struct RecvContext : public IoContext
	{
		RecvContext() : IoContext(IoOperation::Recv)
		{
		}

		void Reset()
		{
			IoContext::Reset();
			recvBuffer.reset();
		}

		std::shared_ptr<std::byte[]> recvBuffer;
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
		}

		size_t sendSize = 0;
	};
}