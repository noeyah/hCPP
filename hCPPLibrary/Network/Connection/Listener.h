#pragma once
#include <memory>
#include <cstdint>
#include <atomic>
#include <vector>
#include <functional>
#include "WinCommon.h"
#include "Network/Core/IoHandler.h"

namespace hlib
{
	struct AcceptContext;
	class SessionManager;

	class Listener 
		: public IoHandler, 
		public std::enable_shared_from_this<Listener>
	{
		friend class WinsockSetup;

	public:
		using AcceptCallback = std::function<void(SOCKET, const SOCKADDR_IN*)>;
		
	private:
		inline static LPFN_ACCEPTEX AcceptEx;
		inline static LPFN_GETACCEPTEXSOCKADDRS GetAcceptExSockAddrs;

		HANDLE m_handle;
		SOCKET m_socket{ INVALID_SOCKET };
		std::atomic_bool m_bRunning{ false };
		std::vector<std::unique_ptr<AcceptContext>> m_lstContext{};
		AcceptCallback m_callback{};

	public:
		Listener(HANDLE handle, AcceptCallback callback) : m_handle(handle), m_callback(callback) {}
		~Listener();

		bool Start(const SOCKADDR_IN& bindAddr, size_t acceptCount, int32_t backlog);
		void Stop();

		virtual void CompletedAsync(IoContext* context, DWORD bytesTransferred) override;
		virtual void ErrorHandler(IoContext* context, int32_t wsaErrorCode) override;

	private:
		void AcceptAsync(AcceptContext* context);
		void AcceptCompleted(AcceptContext* context);
	};

}