#include "SocketUtil.h"
#include "Util/Macro.h"

namespace hlib::sock
{
	SOCKET Create()
	{
		return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	}

	void Close(SOCKET& socket)
	{
		if (socket != INVALID_SOCKET)
		{
			closesocket(socket);
			socket = INVALID_SOCKET;
		}
	}

	void Shutdown(SOCKET& socket, int how)
	{
		// how = SD_BOTH, SD_SEND ...
		shutdown(socket, how);
	}

	bool Bind(SOCKET socket, SOCKADDR_IN address)
	{
		return bind(socket, reinterpret_cast<const SOCKADDR*>(&address), sizeof(address)) != SOCKET_ERROR;
	}

	bool BindAnyAddress(SOCKET socket, uint16_t port)
	{
		SOCKADDR_IN address;
		memset(&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = ::htonl(INADDR_ANY);
		address.sin_port = ::htons(port);

		return Bind(socket, address);
	}

	bool Listen(SOCKET socket, int32_t backlog)
	{
		return listen(socket, backlog) != SOCKET_ERROR;
	}

	bool RegisterIoCompletionPort(HANDLE iocpHandle, SOCKET socket, ULONG_PTR key)
	{
		auto ret = ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), iocpHandle, key, 0);
		return ret != INVALID_HANDLE_VALUE;
	}

	bool LoadExtensionFunction(SOCKET socket, GUID guid, LPVOID* func)
	{
		DWORD dwBytes = 0;
		return WSAIoctl(socket,
						SIO_GET_EXTENSION_FUNCTION_POINTER,
						&guid, sizeof(guid),
						func, sizeof(*func),
						&dwBytes, NULL, NULL) != SOCKET_ERROR;
	}

	bool SetUpdateAcceptContext(SOCKET clientSocket, SOCKET listenSocket)
	{
		return setsockopt(clientSocket,
						  SOL_SOCKET,
						  SO_UPDATE_ACCEPT_CONTEXT,
						  reinterpret_cast<char*>(&listenSocket),
						  sizeof(listenSocket)) != SOCKET_ERROR;
	}

	bool SetUpdateConnectContext(SOCKET connectedSocket)
	{
		return setsockopt(connectedSocket,
						  SOL_SOCKET,
						  SO_UPDATE_CONNECT_CONTEXT,
						  NULL,
						  0) != SOCKET_ERROR;
	}

	bool SetReuseAddress(SOCKET socket, bool reuse)
	{
		return setsockopt(socket,
						  SOL_SOCKET,
						  SO_REUSEADDR,
						  reinterpret_cast<char*>(&reuse),
						  sizeof(reuse)) != SOCKET_ERROR;
	}

	bool SetLinger(SOCKET socket, uint16_t onoff, uint16_t lingerTime)
	{
		LINGER option;
		option.l_onoff = onoff;		// 0이면 끔. 디폴트 0
		option.l_linger = lingerTime;	// 옵션이 켜져있을 때 대기할 시간(초)
		return setsockopt(socket,
						  SOL_SOCKET,
						  SO_LINGER,
						  reinterpret_cast<char*>(&option),
						  sizeof(option)) != SOCKET_ERROR;
	}

	bool SetNoDelay(SOCKET socket, bool enable)
	{
		// 1 : nodelay
		// 0 : nagle
		// 디폴트 0
		return setsockopt(socket,
						  IPPROTO_TCP,
						  TCP_NODELAY,
						  reinterpret_cast<const char*>(&enable),
						  sizeof(enable)) != SOCKET_ERROR;
	}
}