#include "Listener.h"

#include "Network/InternalNetwork.h"
#include "Util/Macro.h"
#include "Network/Socket/SocketUtil.h"
#include "Network/Core/IoContext.h"
#include "Log.h"

namespace hlib
{
	Listener::~Listener()
	{
		Stop();
	}

	bool Listener::Start(const SOCKADDR_IN& bindAddr, size_t acceptCount, int32_t backlog)
	{
		if (m_bRunning.exchange(true))
			return false;

		m_socket = sock::Create();
		ASSERT_CRASH(m_socket != INVALID_SOCKET);

		if (!sock::SetReuseAddress(m_socket, true))
			CRASH("listener socket reuse failed - {}", ::WSAGetLastError());

		if (!sock::Bind(m_socket, bindAddr))
			CRASH("listener socket bind failed - {}", ::WSAGetLastError());

		if (!sock::Listen(m_socket, backlog))
			CRASH("listener socket listen failed - {}", ::WSAGetLastError());

		if (!sock::RegisterIoCompletionPort(m_handle, m_socket, 0))
			CRASH("listener socket register io failed - {}", ::WSAGetLastError());
		
		m_lstContext.reserve(acceptCount);
		for (size_t i = 0; i < acceptCount; i++)
		{
			m_lstContext.emplace_back(std::make_unique<AcceptContext>());
			AcceptAsync(m_lstContext.back().get());
		}

		LOG_INFO("listener start {}", acceptCount);
		return true;
	}

	void Listener::Stop()
	{
		if (!m_bRunning.exchange(false))
			return;

		CancelIoEx(reinterpret_cast<HANDLE>(m_socket), NULL);
		sock::Close(m_socket);
		LOG_INFO("listener stop");
	}

	void Listener::CompletedAsync(IoContext* context, DWORD bytesTransferred)
	{
		if (context->ioOperation != IoOperation::Accept)
			return;

		AcceptCompleted(static_cast<AcceptContext*>(context));
	}

	void Listener::ErrorHandler(IoContext* context, int32_t wsaErrorCode)
	{
		if (wsaErrorCode == ERROR_OPERATION_ABORTED)
			LOG_WARN("listener accept cancel");
		else
			LOG_ERROR("listener io error : {}", wsaErrorCode);
	}

	void Listener::AcceptAsync(AcceptContext* acceptContext)
	{
		acceptContext->Reset();

		if (!m_bRunning.load())
			return;

		SOCKET sock = sock::Create();
		ASSERT_CRASH(sock != INVALID_SOCKET);

		acceptContext->ioHandler = shared_from_this();
		acceptContext->clientSocket = sock;

		DWORD bytesReceived = 0;
		auto ret = AcceptEx(m_socket,
							acceptContext->clientSocket,
							acceptContext->clientAddressBuffer.data(),
							0,
							sizeof(SOCKADDR_IN) + SOCKADDR_PADDING,
							sizeof(SOCKADDR_IN) + SOCKADDR_PADDING,
							&bytesReceived,
							static_cast<LPOVERLAPPED>(acceptContext));

		if (!ret)
		{
			const int errorCode = WSAGetLastError();
			if (errorCode != WSA_IO_PENDING)
			{
				LOG_ERROR("accept error : {}", errorCode);
				sock::Close(sock);
				return;
			}
		}
	}

	void Listener::AcceptCompleted(AcceptContext* context)
	{
		SOCKET sock = context->clientSocket;

		if (!sock::SetUpdateAcceptContext(sock, m_socket))
		{
			CRASH("SetUpdateAcceptContext failed - {}", ::WSAGetLastError());
		}

		LPSOCKADDR localAddr = nullptr;
		LPSOCKADDR remoteAddr = nullptr;
		int32_t localAddrLength = 0;
		int32_t remoteAddrLength = 0;

		GetAcceptExSockAddrs(context->clientAddressBuffer.data(),
							 0,
							 sizeof(SOCKADDR_IN) + SOCKADDR_PADDING,
							 sizeof(SOCKADDR_IN) + SOCKADDR_PADDING,
							 &localAddr,
							 &localAddrLength,
							 &remoteAddr,
							 &remoteAddrLength);

		auto addr = reinterpret_cast<SOCKADDR_IN*>(remoteAddr);
		if (m_callback)
			m_callback(sock, addr);

		AcceptAsync(context);
	}

}