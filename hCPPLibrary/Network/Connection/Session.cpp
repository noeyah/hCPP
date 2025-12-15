#include "Session.h"
#include "Memory/Memory.h"
#include "Memory/Buffer.h"
#include "Network/Socket/SocketUtil.h"
#include "Util/Macro.h"
#include "Network/Buffer/PacketBuffer.h"
#include "Log.h"

namespace hlib
{

	Session::~Session()
	{
		m_sendBuffer.Clear();
		sock::Close(m_socket);
	}

	void Session::Initialize(SOCKET socket, uint64_t sessionId, const SOCKADDR_IN& sock_addr)
	{
		m_socket = socket;
		m_id = sessionId;
		m_address = sock_addr;
	}

	void Session::StartIo()
	{
		if (!TryOpen())
			return;

		OnConnected();
		RecvAsync();
	}

	bool Session::Send(std::shared_ptr<PacketBuffer> pBuffer)
	{
		bool startSending = false;

		if (pBuffer->DataSize() > MAX_BUFFER_SIZE)
			return false;

		m_sendBuffer.Push(pBuffer);

		if (m_sendBuffer.Begin())
			SendAsync();

		return true;
	}

	void Session::CompletedAsync(IoContext* context, DWORD bytesTransferd)
	{
		if (!context)
			return;

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
			m_recvContext.Reset();
			break;

		case IoOperation::Send:
			m_sendContext.Reset();
			break;

		case IoOperation::Connect:
			m_connectContext.Reset();
			break;
		}

		switch (wsaErrorCode)
		{
		case ERROR_NETNAME_DELETED:
		case WSAECONNRESET:
		case WSAECONNABORTED:
		{
			LOG_DEBUG("session({}) disconnected.", GetID());
			Close();
		} break;
		default:
			LOG_ERROR("session({}) io error : {}", GetID(), wsaErrorCode);
			break;
		}
	}

	void Session::Close()
	{
		m_bConnected.store(false);

		if (m_disconnectCallback)
			m_disconnectCallback(m_id);
	}

	void Session::ConnectAsync()
	{
		if (IsConnected())
			return;

		if (!SetConnectSockopt())
		{
			Close();
			return;
		}

		m_connectContext.Reset();
		m_connectContext.ioHandler = shared_from_this();

		DWORD bytes_sent = 0;

		auto ret = ConnectEx(m_socket,
							 reinterpret_cast<const SOCKADDR*>(&m_address),
							 sizeof(m_address),
							 nullptr,
							 0,
							 &bytes_sent,
							 static_cast<LPOVERLAPPED>(&m_connectContext));

		if (!ret)
		{
			const int errorCode = WSAGetLastError();
			if (errorCode != WSA_IO_PENDING)
			{
				LOG_ERROR("session({}) connect error : {}", GetID(), errorCode);
				Close();
				return;
			}
		}
	}

	void Session::ConnectCompleted()
	{
		m_connectContext.Reset();

		if (!sock::SetUpdateConnectContext(m_socket))
		{
			CRASH("SetUpdateConnectContext failed - {}", ::WSAGetLastError());
		}

		StartIo();
	}

	void Session::RecvAsync()
	{
		if (!IsConnected())
			return;

		m_recvContext.Reset();
		m_recvContext.ioHandler = shared_from_this();
		
		m_recvBuffer.Clean();

		if (m_recvBuffer.FreeSize() == 0)
		{
			LOG_WARN("session({}) recv buffer overflow : {}", GetID());
			Close();
			return;
		}

		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(m_recvBuffer.WritePos());
		wsaBuf.len = static_cast<ULONG>(m_recvBuffer.FreeSize());

		DWORD numOfBytes = 0;
		DWORD flag = 0;

		auto ret = WSARecv(m_socket,
						   &wsaBuf,
						   1,
						   &numOfBytes,
						   &flag,
						   &m_recvContext,
						   nullptr);

		if (ret == SOCKET_ERROR)
		{
			auto errorCode = WSAGetLastError();
			if (errorCode != WSA_IO_PENDING)
			{
				LOG_ERROR("session({}) recv error : {}", GetID(), errorCode);
				m_recvContext.Reset();
				Close();
				return;
			}
		}
	}

	void Session::RecvCompleted(DWORD bytesTransferred)
	{
		if (bytesTransferred == 0 || !IsConnected())
		{
			LOG_INFO("session({}) disconnected or recv 0", GetID());
			m_recvContext.Reset();
			Close();
			return;
		}
		
		if (!m_recvBuffer.OnWrite(bytesTransferred))
		{
			LOG_ERROR("session({}) buffer write overflow error", GetID());
			Close();
			return;
		}

		ReadBuffer();
		RecvAsync();
	}

	void Session::SendAsync()
	{
		if (!IsConnected())
		{
			m_sendBuffer.End();
			return;
		}

		if (m_sendBuffer.IsEmpty())
		{
			m_sendBuffer.End();
			return;
		}

		m_sendContext.Reset();
		m_sendContext.ioHandler = shared_from_this();

		size_t sendBytes;
		m_sendBuffer.PrepareSend(m_sendContext.wsaBufs, MAX_SEND_BUFFER_COUNT, m_sendContext.sendSize, sendBytes);

		DWORD numOfBytes = 0;
		auto ret = WSASend(m_socket,
						   m_sendContext.wsaBufs.data(),
						   static_cast<DWORD>(m_sendContext.wsaBufs.size()),
						   &numOfBytes,
						   0,
						   &m_sendContext,
						   nullptr);

		if (ret == SOCKET_ERROR)
		{
			auto errorCode = WSAGetLastError();
			if (errorCode != WSA_IO_PENDING)
			{
				LOG_ERROR("session({}) send error : {}", GetID(), errorCode);
				m_sendContext.Reset();
				m_sendBuffer.End();
				Close();
				return;
			}
		}
	}

	void Session::SendCompleted(DWORD bytesTransferred)
	{
		if (bytesTransferred == 0)
		{
			LOG_WARN("session({}) completed send bytes 0", GetID());
			m_sendContext.Reset();
			m_sendBuffer.End();
			Close();
			return;
		}

		if (!IsConnected())
		{
			m_sendContext.Reset();
			m_sendBuffer.End();
			return;
		}

		m_sendBuffer.CompleteSend(m_sendContext.sendSize);
		m_sendContext.Reset();

		SendAsync();
	}

	bool Session::SetConnectSockopt()
	{
		if (!sock::SetReuseAddress(m_socket, true))
		{
			CRASH("connect socket reuse failed");
			return false;
		}

		if (!sock::BindAnyAddress(m_socket, 0))
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
			if (m_recvBuffer.DataSize() < HEADER_SIZE)
				break;

			PacketHeader* header = reinterpret_cast<PacketHeader*>(m_recvBuffer.ReadPos());

			if (header->size > m_recvBuffer.Capacity())
			{
				LOG_ERROR("session({}) packet too large: {}", GetID(), header->size);
				Close();
				return;
			}

			// 패킷이 다 도착하면 조립해야함
			if (m_recvBuffer.DataSize() < header->size)
				break;

			std::byte* payloadPtr = m_recvBuffer.ReadPos() + HEADER_SIZE;
			size_t payloadSize = header->size - HEADER_SIZE;

			std::span<std::byte> payload(payloadPtr, payloadSize);

			OnReceive(header->id, payload);

			if (m_recvBuffer.OnRead(header->size) == false)
			{
				LOG_ERROR("session({}) error in OnRead", GetID());
				Close();
				return;
			}
		}
	}

}