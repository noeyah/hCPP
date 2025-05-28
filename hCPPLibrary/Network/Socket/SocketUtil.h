#pragma once
#include <iostream>
#include "WinCommon.h"

namespace hlib::net::sock
{
	SOCKET Create();
	void Close(SOCKET& socket);
	void Shutdown(SOCKET& socket, int how);

	bool Bind(SOCKET socket, SOCKADDR_IN sockAddr);
	bool BindAnyAddress(SOCKET socket, uint16_t port);
	bool Listen(SOCKET socket, int32_t backlog = SOMAXCONN);
	bool RegisterIoCompletionPort(HANDLE iocpHandle, SOCKET socket, ULONG_PTR key);

	bool LoadExtensionFunction(SOCKET socket, GUID guid, LPVOID* func);

	bool SetUpdateAcceptContext(SOCKET clientSocket, SOCKET listenSocket);
	bool SetUpdateConnectContext(SOCKET connectedSocket);
	bool SetReuseAddress(SOCKET socket, bool reuse);
	bool SetLinger(SOCKET socket, uint16_t onoff, uint16_t lingerTime = 0);
	bool SetNoDelay(SOCKET socket, bool enable);

};