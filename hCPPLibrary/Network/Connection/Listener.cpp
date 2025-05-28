#include "Listener.h"

#include "Network/InternalNetwork.h"
#include "Util/Macro.h"
#include "Network/Socket/SocketUtil.h"
#include "Network/Core/IoContext.h"
#include "Network/Socket/SocketPool.h"
#include "Network/Connection/SessionManager.h"
#include "Log.h"

namespace hlib::net
{
	Listener::Listener(HANDLE handle, SocketPool& sockProvider, SessionManager& sessionManager)
		: iocpHandle_(handle), socketPool_(sockProvider), sessionManager_(sessionManager)
	{
	}

	Listener::~Listener()
	{
		Stop();
	}

	bool Listener::Start(const SOCKADDR_IN& bindAddr, size_t acceptCount, int32_t backlog)
	{
		if (isListening_.exchange(true))
			return false;

		listenSocket_ = sock::Create();
		ASSERT_CRASH(listenSocket_ != INVALID_SOCKET);

		if (!sock::SetReuseAddress(listenSocket_, true))
		{
			CRASH("listener socket reuse failed - {}", ::WSAGetLastError());
		}

		if (!sock::Bind(listenSocket_, bindAddr))
		{
			CRASH("listener socket bind failed - {}", ::WSAGetLastError());
		}

		if (!sock::Listen(listenSocket_, backlog))
		{
			CRASH("listener socket listen failed - {}", ::WSAGetLastError());
		}

		if (!sock::RegisterIoCompletionPort(iocpHandle_, listenSocket_, 0))
		{
			CRASH("listener socket register io failed - {}", ::WSAGetLastError());
		}
		
		acceptContexts_.reserve(acceptCount);
		for (size_t i = 0; i < acceptCount; i++)
		{
			acceptContexts_.emplace_back(std::make_unique<AcceptContext>());
			AcceptAsync(acceptContexts_.back().get());
		}

		LOG_INFO("listener start {}", acceptCount);
		return true;
	}

	void Listener::Stop()
	{
		if (!isListening_.exchange(false))
			return;

		CancelIoEx(reinterpret_cast<HANDLE>(listenSocket_), NULL);
		sock::Close(listenSocket_);
		LOG_INFO("listener stop");
	}

	void Listener::CompletedAsync(IoContext* context, DWORD bytesTransferd)
	{
		if (context->ioOperation != IoOperation::Accept)
		{
			return;
		}

		AcceptCompleted(static_cast<AcceptContext*>(context));
	}

	void Listener::ErrorHandler(IoContext* context, int32_t wsaErrorCode)
	{
		if (wsaErrorCode == ERROR_OPERATION_ABORTED)
		{
			LOG_WARN("listener accept cancel");
		}
		else
		{
			LOG_ERROR("listener io error : {}", wsaErrorCode);
		}
	}

	void Listener::AcceptAsync(AcceptContext* acceptContext)
	{
		acceptContext->Reset();

		if (!isListening_.load())
		{
			return;
		}

		SOCKET clientSocket;
		if (!socketPool_.TryGet(&clientSocket))
		{
			LOG_WARN("socket provider is empty");
			return;
		}

		acceptContext->ioHandler = shared_from_this();
		acceptContext->clientSocket = clientSocket;

		DWORD bytesReceived = 0;
		auto ret = AcceptEx(listenSocket_,
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
				sock::Close(clientSocket);
				return;
			}
		}
	}

	void Listener::AcceptCompleted(AcceptContext* context)
	{
		SOCKET clientSocket = context->clientSocket;

		if (!sock::SetUpdateAcceptContext(clientSocket, listenSocket_))
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

		auto clientAddr = reinterpret_cast<SOCKADDR_IN*>(remoteAddr);
		sessionManager_.CreateAndStart(clientSocket, clientAddr);

		AcceptAsync(context);
	}

}