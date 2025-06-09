#include "SessionManager.h"
#include <vector>
#include "Util/Macro.h"
#include "Network/Socket/SocketUtil.h"

namespace hlib::net
{
	SessionManager::SessionManager(HANDLE handle, SessionFactory factory)
		: iocpHandle_(handle), sessionFactory_(std::move(factory))
	{
		ASSERT_CRASH(sessionFactory_);
	}

	SessionManager::~SessionManager()
	{
		sessions_.clear();
	}

	void SessionManager::SetReturnSockCallback(Session::ReturnSockCallback sockReturnCallback)
	{
		sockReturnCallback_ = sockReturnCallback;
	}

	std::shared_ptr<Session> SessionManager::Create(SOCKET socket, const SOCKADDR_IN* sockAddr)
	{
		auto sessionId = GenerateSessionId();
		auto removeCallback = [this](SessionId removeId) {
			this->Remove(removeId);
		};
		
		std::shared_ptr<Session> session = sessionFactory_();
		session->Initialize(socket, sessionId, *sockAddr, removeCallback);

		if (sockReturnCallback_)
		{
			session->SetReturnSock(sockReturnCallback_);
		}

		if (!sock::RegisterIoCompletionPort(iocpHandle_, socket, sessionId))
		{
			CRASH("RegisterIoCompletionPort failed - {}", ::WSAGetLastError());
		}

		{
			Lock lock(sspin_);
			sessions_[sessionId] = session;
		}
		return session;
	}

	void SessionManager::CreateAndStart(SOCKET socket, const SOCKADDR_IN* sockAddr)
	{
		auto session = Create(socket, sockAddr);
		if (session)
			session->StartIo();
	}

	std::shared_ptr<Session> SessionManager::Get(SessionId sessionId)
	{
		SharedLock lock(sspin_);

		auto it = sessions_.find(sessionId);
		if (it == sessions_.end())
			return nullptr;

		return it->second;
	}

	void SessionManager::Close()
	{
		std::vector<std::shared_ptr<Session>> sessions;

		{
			SharedLock lock(sspin_);
			sessions.reserve(sessions_.size());
			for (auto& [id, session] : sessions_)
			{
				if (session)
					sessions.push_back(session);
			}
		}

		for (auto& session : sessions)
		{
			if (session)
				session->Disconnect();
		}
	}

	void SessionManager::Remove(uint64_t sessionId)
	{
		Lock lock(sspin_);

		auto it = sessions_.find(sessionId);
		if (it == sessions_.end())
			return;

		sessions_.erase(it);
	}

}