#include "Session.h"
#include "Memory/Memory.h"
#include "Memory/Buffer.h"
#include "Network/Socket/SocketUtil.h"
#include "Util/Macro.h"
#include "Network/Buffer/PacketBuffer.h"
#include "Log.h"

namespace hlib::net
{
	Session::Session()
	{
#ifdef _DEBUG
		constructionCnt_++;
#endif // _DEBUG
	}
	Session::~Session()
	{
#ifdef _DEBUG
		destructionCnt_++;
#endif // _DEBUG

		bufferReader_.Clear();
		sendQueue_.Clear();

		if (returnSockCallback_)
		{
			returnSockCallback_(socket_);
		}
		else
		{
			sock::Close(socket_);
		}
	}

#ifdef _DEBUG
	void Session::Print()
	{
		LOG_DEBUG("Session counting {} : {}", constructionCnt_.load(), destructionCnt_.load());
	}
#endif // _DEBUG

	void Session::Initialize(SOCKET socket,
							 SessionId sessionId,
							 const SOCKADDR_IN& sock_addr, 
							 DisconnectCallback disconnectCallback)
	{
		socket_ = socket;
		sessionId_ = sessionId;
		sockAddress_ = sock_addr;
		state_.store(State::State_Idle);

		disconnectCallback_ = std::move(disconnectCallback);
	}

	void Session::SetReturnSock(ReturnSockCallback returnSockCallback)
	{
		returnSockCallback_ = returnSockCallback;
	}

	void Session::StartIo()
	{
		state_.store(State::State_Connected);

		OnConnected();

		RecvAsync();
	}

	void Session::Connect()
	{
		ConnectAsync();
	}

	void Session::Disconnect()
	{
		DisconnectAsync();
	}

	bool Session::Send(std::shared_ptr<PacketBuffer> buffer)
	{
		bool startSending = false;

		if (buffer->Size() > MAX_BUFFER_SIZE)
		{
			return false;
		}

		bool startSend = false;
		{
			std::lock_guard lock(sendMtx_);
			sendQueue_.Push(buffer);

			if (!isSending_.exchange(true))
				startSend = true;
		}

		if (startSend)
		{
			SendAsync();
		}
		
		return true;
	}

	void Session::CompletedAsync(IoContext* context, DWORD bytesTransferd)
	{
		if (!context)
		{
			return;
		}

		switch (context->ioOperation)
		{
		case IoOperation::Connect:
			ConnectCompleted();
			break;

		case IoOperation::Recv:
			RecvCompleted(bytesTransferd);
			break;

		case IoOperation::Send:
			SendCompleted(bytesTransferd);
			break;

		case IoOperation::Disconnect:
			DisconnectCompleted();
			break;

		default:
			LOG_WARN("unknown ioOperation");
			break;
		}
	}

	void Session::ErrorHandler(IoContext* context, int32_t wsaErrorCode)
	{
		switch (context->ioOperation)
		{
		case IoOperation::Recv:
			recvContext_.Reset();
			break;

		case IoOperation::Send:
			sendContext_.Reset();
			break;

		case IoOperation::Connect:
			connectContext_.Reset();
			break;

		case IoOperation::Disconnect:
			disconnectContext_.Reset();
			break;
		}

		switch (wsaErrorCode)
		{
		case ERROR_NETNAME_DELETED:
		case WSAECONNRESET:
		case WSAECONNABORTED:
		{
			LOG_DEBUG("session({}) disconnected. state : {}", GetSessionId(), state_.load());
			Disconnect();
		} break;
		default:
			LOG_ERROR("session({}) io error : {}", GetSessionId(), wsaErrorCode);
			break;
		}
	}

	void Session::ConnectAsync()
	{
		if (state_.exchange(State::State_Connecting) != State::State_Idle)
			return;

		if (!SetConnectSockopt())
		{
			sock::Close(socket_);
			
			return;
		}

		connectContext_.Reset();
		connectContext_.ioHandler = shared_from_this();

		DWORD bytes_sent = 0;

		auto ret = ConnectEx(socket_,
							 reinterpret_cast<const SOCKADDR*>(&sockAddress_),
							 sizeof(sockAddress_),
							 nullptr,
							 0,
							 &bytes_sent,
							 static_cast<LPOVERLAPPED>(&connectContext_));

		if (!ret)
		{
			const int errorCode = WSAGetLastError();
			if (errorCode != WSA_IO_PENDING)
			{
				LOG_ERROR("session({}) connect error : {}", GetSessionId(), errorCode);
				sock::Close(socket_);
				RemoveSession();
				return;
			}
		}
	}

	void Session::ConnectCompleted()
	{
		connectContext_.Reset();

		if (!sock::SetUpdateConnectContext(socket_))
		{
			CRASH("SetUpdateConnectContext failed - {}", ::WSAGetLastError());
		}

		StartIo();
	}

	void Session::DisconnectAsync()
	{
		if (state_.exchange(State::State_Disconnect) == State::State_Disconnect)
		{
			return;
		}

		disconnectContext_.Reset();
		disconnectContext_.ioHandler = shared_from_this();

		auto ret = DisconnectEx(socket_,
								&disconnectContext_,
								TF_REUSE_SOCKET,
								0);

		if (!ret)
		{
			const int errorCode = WSAGetLastError();
			if (errorCode != WSA_IO_PENDING)
			{
				LOG_ERROR("session({}) disconnect error : {}", GetSessionId(), errorCode);
				disconnectContext_.Reset();
				sock::Close(socket_);
				RemoveSession();
				return;
			}
		}
	}

	void Session::DisconnectCompleted()
	{
		disconnectContext_.Reset();

		OnDisconnected();

		RemoveSession();
	}

	void Session::RecvAsync()
	{
		recvContext_.Reset();

		if (!IsConnected())
		{
			return;
		}

		recvContext_.ioHandler = shared_from_this();
		recvContext_.recvBuffer = Buffer::Get(MAX_BUFFER_SIZE);
		
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(recvContext_.recvBuffer.get());
		wsaBuf.len = static_cast<ULONG>(MAX_BUFFER_SIZE);

		DWORD numOfBytes = 0;
		DWORD flag = 0;

		auto ret = WSARecv(socket_,
						   &wsaBuf,
						   1,
						   &numOfBytes,
						   &flag,
						   &recvContext_,
						   nullptr);

		if (ret == SOCKET_ERROR)
		{
			auto errorCode = WSAGetLastError();
			if (errorCode != WSA_IO_PENDING)
			{
				LOG_ERROR("session({}) recv error : {}", GetSessionId(), errorCode);
				recvContext_.Reset();
				Disconnect();
				return;
			}
		}
	}

	void Session::RecvCompleted(DWORD bytesTransferred)
	{
		if (bytesTransferred == 0)
		{
			LOG_INFO("session({}) recv 0", GetSessionId());
			recvContext_.Reset();
			Disconnect();
			return;
		}

		if (!IsConnected())
		{
			recvContext_.Reset();
			return;
		}
		
		auto buffer = recvContext_.recvBuffer.get();

		if (!bufferReader_.Write(reinterpret_cast<const void*>(buffer), bytesTransferred))
		{
			// 버퍼 부족
			LOG_WARN("session({}) buffer reader write failed. recv bytes : {}", GetSessionId(), bytesTransferred);
			recvContext_.Reset();
			return;
		}

		ReadBuffer();
		bufferReader_.Cleanup();

		recvContext_.Reset();
		RecvAsync();
	}

	void Session::SendAsync()
	{
		if (!IsConnected())
		{
			SendEnd();
			return;
		}

		{
			std::lock_guard lock(sendMtx_);
			if (sendQueue_.IsEmpty())
			{
				SendEnd();
				return;
			}
		}

		sendContext_.Reset();
		sendContext_.ioHandler = shared_from_this();

		size_t sendBytes;
		sendQueue_.PrepareSend(sendContext_.wsaBufs, MAX_SEND_BUFFER_COUNT, sendContext_.sendSize, sendBytes);

		DWORD numOfBytes = 0;
		auto ret = WSASend(socket_,
						   sendContext_.wsaBufs.data(),
						   static_cast<DWORD>(sendContext_.wsaBufs.size()),
						   &numOfBytes,
						   0,
						   &sendContext_,
						   nullptr);

		if (ret == SOCKET_ERROR)
		{
			auto errorCode = WSAGetLastError();
			if (errorCode != WSA_IO_PENDING)
			{
				LOG_ERROR("session({}) send error : {}", GetSessionId(), errorCode);
				sendContext_.Reset();
				SendEnd();
				Disconnect();
				return;
			}
		}
	}

	void Session::SendCompleted(DWORD bytesTransferred)
	{
		if (bytesTransferred == 0)
		{
			LOG_WARN("session({}) completed send bytes 0", GetSessionId());
			sendContext_.Reset();
			SendEnd();
			Disconnect();
			return;
		}

		if (!IsConnected())
		{
			sendContext_.Reset();
			SendEnd();
			return;
		}

		sendQueue_.CompleteSend(sendContext_.sendSize);
		sendContext_.Reset();

		SendAsync();
	}

	bool Session::SetConnectSockopt()
	{
		if (!sock::SetReuseAddress(socket_, true))
		{
			CRASH("connect socket reuse failed");
			return false;
		}

		if (!sock::BindAnyAddress(socket_, 0))
		{
			CRASH("connect socket bind failed");
			return false;
		}

		return true;
	}

	void Session::ReadBuffer()
	{
		while (true)
		{
			auto bufferSize = bufferReader_.GetDataSize();
			if (bufferSize < HEADER_SIZE)
				break;

			auto buffer = bufferReader_.GetReadBuffer();
			PacketHeader header;
			memcpy(&header, buffer, HEADER_SIZE);

			ASSERT_CRASH(header.size <= MAX_BUFFER_SIZE);
			if (bufferSize < header.size)
				break;

			std::span<const std::byte> packet(buffer + HEADER_SIZE, header.size - HEADER_SIZE);

			OnReceive(header.id, packet);
			bufferReader_.CommitRead(header.size);
		}
	}

	void Session::SendEnd()
	{
		isSending_.store(false);
	}

	void Session::RemoveSession()
	{
		if (disconnectCallback_)
			disconnectCallback_(sessionId_);
	}

	/*void Session::LogHex(const char* title, const void* data, size_t size)
	{
		std::cout << title << " (" << size << " bytes): ";
		const unsigned char* p = static_cast<const unsigned char*>(data);
		for (size_t i = 0; i < size && i < 32; ++i) {
			std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)p[i] << " ";
		}
		std::cout << std::dec << std::endl;
	}*/

}