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
#include "Network/Core/IIoCompletionHandler.h"
#include "Network/Core/IoContext.h"
#include "Network/Buffer/BufferReader.h"
#include "Network/Buffer/AsyncSendQueue.h"

namespace hlib::net
{
	class PacketBuffer;

	class Session : public IIoCompletionHandler, public std::enable_shared_from_this<Session>
	{
		friend class WinsockSetup;

	public:
		using DisconnectCallback = std::function<void(SessionId)>;
		using ReturnSockCallback = std::function<void(SOCKET)>;
		using SharedPtr = std::shared_ptr<Session>;

	private:
		enum State : uint8_t
		{
			State_Idle,
			State_Connecting,
			State_Connected,
			State_Disconnect,
			State_Close,
		};

		inline static constexpr size_t MAX_SEND_BUFFER_COUNT = 10;

	private:
		inline static LPFN_CONNECTEX ConnectEx;
		inline static LPFN_DISCONNECTEX DisconnectEx;

	public:
		Session();
		virtual ~Session();

		void Initialize(SOCKET socket, 
						SessionId sessionId,
						const SOCKADDR_IN& sock_addr,
						DisconnectCallback disconnectCallback);
		void SetReturnSock(ReturnSockCallback returnSockCallback);
		void StartIo();

		void Connect();
		void Disconnect();

		bool Send(std::shared_ptr<PacketBuffer> buffer);

		virtual void CompletedAsync(IoContext* context, DWORD bytesTransferd) override;
		virtual void ErrorHandler(IoContext* context, int32_t wsaErrorCode) override;

		SessionId GetSessionId() const { return sessionId_; }
		bool IsConnected() const { return state_.load() == State::State_Connected; }

#ifdef _DEBUG
		static void Print();
#endif // _DEBUG

	protected:
		// 사용자 정의 필요
		virtual void OnConnected() = 0;
		virtual void OnDisconnected() = 0;
		virtual void OnReceive(uint16_t packetId, std::span<const std::byte> packet) = 0;

	private:
		void ConnectAsync();
		void ConnectCompleted();

		void DisconnectAsync();
		void DisconnectCompleted();

		void RecvAsync();
		void RecvCompleted(DWORD bytesTransferred);

		void SendAsync();
		void SendCompleted(DWORD bytesTransferred);

	private:
		bool SetConnectSockopt();
		void ReadBuffer();
		void SendEnd();
		void RemoveSession();

	private:
		SOCKET socket_ = INVALID_SOCKET;
		SessionId sessionId_ = 0;
		SOCKADDR_IN sockAddress_{};
		std::atomic_uint8_t state_{ State::State_Idle };

		DisconnectCallback disconnectCallback_ = nullptr;
		ReturnSockCallback returnSockCallback_ = nullptr;

		// connect
		IoContext connectContext_{ IoOperation::Connect };

		// disconnect
		IoContext disconnectContext_{ IoOperation::Disconnect };

		// receive
		RecvContext recvContext_;
		BufferReader bufferReader_;

		// send
		SendContext sendContext_;
		AsyncSendQueue sendQueue_;
		std::atomic_bool isSending_ = false;
		std::recursive_mutex sendMtx_;

#ifdef _DEBUG
		inline static std::atomic_uint64_t constructionCnt_ = 0;
		inline static std::atomic_uint64_t destructionCnt_ = 0;
#endif // _DEBUG
	};

}