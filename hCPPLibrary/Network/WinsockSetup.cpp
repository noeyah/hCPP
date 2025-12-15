#include "WinsockSetup.h"
#include "WinCommon.h"
#include "Util/Macro.h"
#include "Network/Socket/SocketUtil.h"
#include "Network/Connection/Listener.h"
#include "Network/Connection/Session.h"

namespace hlib
{
	WinsockSetup::WinsockSetup()
	{
		WSADATA wsaData;
		auto ret = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (ret != 0)
		{
			CRASH("WSAStartup failed - {}", ::WSAGetLastError());
		}

		SOCKET tmp = sock::Create();
		if (tmp == INVALID_SOCKET)
		{
			CRASH("WinsockSetup : create socket failed - {}", ::WSAGetLastError());
		}

		if (!sock::LoadExtensionFunction(tmp, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&Listener::AcceptEx)))
		{
			CRASH("WSAID_ACCEPTEX failed - {}", ::WSAGetLastError());
		}

		if (!sock::LoadExtensionFunction(tmp, WSAID_GETACCEPTEXSOCKADDRS, reinterpret_cast<LPVOID*>(&Listener::GetAcceptExSockAddrs)))
		{
			CRASH("WSAID_GETACCEPTEXSOCKADDRS failed - {}", ::WSAGetLastError());
		}
		
		if (!sock::LoadExtensionFunction(tmp, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&Session::ConnectEx)))
		{
			CRASH("WSAID_CONNECTEX failed - {}", ::WSAGetLastError());
		}

		sock::Close(tmp);
	}

	WinsockSetup::~WinsockSetup()
	{
		WSACleanup();
	}
}