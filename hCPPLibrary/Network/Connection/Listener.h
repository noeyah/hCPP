#pragma once
#include <memory>
#include <cstdint>
#include <atomic>
#include <vector>
#include "WinCommon.h"
#include "Network/Core/IIoCompletionHandler.h"

namespace hlib::net
{
	struct AcceptContext;

	class SocketPool;
	class SessionManager;

	class Listener : public IIoCompletionHandler, public std::enable_shared_from_this<Listener>
	{
		friend class WinsockSetup;

	private:
		inline static LPFN_ACCEPTEX AcceptEx;
		inline static LPFN_GETACCEPTEXSOCKADDRS GetAcceptExSockAddrs;

	public:
		Listener(HANDLE handle, SocketPool& sockProvider, SessionManager& sessionManager);
		~Listener();

		bool Start(const SOCKADDR_IN& bindAddr, size_t acceptCount, int32_t backlog);
		void Stop();

		virtual void CompletedAsync(IoContext* context, DWORD bytesTransferred) override;
		virtual void ErrorHandler(IoContext* context, int32_t wsaErrorCode) override;

	private:
		void AcceptAsync(AcceptContext* context);
		void AcceptCompleted(AcceptContext* context);

	private:
		SOCKET listenSocket_ = INVALID_SOCKET;
		std::vector<std::unique_ptr<AcceptContext>> acceptContexts_;
		std::atomic_bool isListening_{ false };

		HANDLE iocpHandle_;
		SocketPool& socketPool_;
		SessionManager& sessionManager_;
	};

}