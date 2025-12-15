#pragma once
#include <memory>
#include <cstdint>
#include <atomic>
#include <functional>
#include <cstddef>
#include <mutex>
#include <span>

#include "WinCommon.h"
#include "Network/NetCommon.h"
#include "Network/InternalNetwork.h"
#include "Network/Core/IoHandler.h"
#include "Network/Core/IoContext.h"
#include "Network/Buffer/SendBuffer.h"
#include "Network/Buffer/PacketBuffer.h"

namespace hlib
{
	class PacketBuffer;

	class Session : public IoHandler, public std::enable_shared_from_this<Session>
	{
		friend class WinsockSetup;
		friend class SessionManager;
		friend class NetClient;

	public:
		using DisconnectCallback = std::function<void(uint64_t)>;
		using SharedPtr = std::shared_ptr<Session>;

	private:
		inline static LPFN_CONNECTEX ConnectEx;

		SOCKET m_socket{ INVALID_SOCKET };
		uint64_t m_id{ 0 };
		SOCKADDR_IN m_address{};
		std::atomic_bool m_bConnected{ false };
		DisconnectCallback m_disconnectCallback{};

		IoContext m_connectContext{ IoOperation::Connect };
		IoContext m_recvContext{ IoOperation::Recv };
		PacketBuffer m_recvBuffer{ MAX_BUFFER_SIZE * 2 };
		SendContext m_sendContext{};
		SendBuffer m_sendBuffer{};

	public:
		Session() = default;
		virtual ~Session();

		bool Send(std::shared_ptr<PacketBuffer> pBuffer);

		uint64_t GetID() const { return m_id; }
		bool IsConnected() const { return m_bConnected.load(); }

	protected:
		virtual void OnConnected() = 0;
		virtual void OnDisconnected() = 0;
		virtual void OnReceive(uint16_t packetId, std::span<const std::byte> packet) = 0;

	private:
		void SetDisconnectCallback(DisconnectCallback callback) { m_disconnectCallback = std::move(callback); }

		void Initialize(SOCKET socket, uint64_t sessionId, const SOCKADDR_IN& sock_addr);
		void StartIo();
		
		bool TryOpen() { return !m_bConnected.exchange(true); }
		void Close();

		virtual void CompletedAsync(IoContext* context, DWORD bytesTransferd) final;
		virtual void ErrorHandler(IoContext* context, int32_t wsaErrorCode) final;

		void ConnectAsync();
		void ConnectCompleted();

		void RecvAsync();
		void RecvCompleted(DWORD bytesTransferred);

		void SendAsync();
		void SendCompleted(DWORD bytesTransferred);

		bool SetConnectSockopt();
		void ReadBuffer();
	};

}