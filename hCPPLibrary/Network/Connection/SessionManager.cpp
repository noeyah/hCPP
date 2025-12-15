#include "SessionManager.h"
#include <vector>
#include "Util/Macro.h"
#include "Network/Socket/SocketUtil.h"
#include "Network/Connection/Session.h"

namespace hlib
{
	SessionManager::~SessionManager()
	{
		m_sessions.clear();
	}

	SessionPtr SessionManager::Create(SOCKET socket, const SOCKADDR_IN* sockAddr)
	{
		if (!m_sessionFactory)
			return nullptr;

		auto id = GenerateID();
		SessionPtr session = m_sessionFactory();
		session->Initialize(socket, id, *sockAddr);
		session->SetDisconnectCallback([this](uint64_t id) { this->Remove(id); });

		if (!sock::RegisterIoCompletionPort(m_handle, socket, id))
		{
			CRASH("RegisterIoCompletionPort failed - {}", ::WSAGetLastError());
		}

		{
			Lock lock(m_lock);
			m_sessions[id] = session;
		}

		return session;
	}

	void SessionManager::CreateAndStart(SOCKET socket, const SOCKADDR_IN* sockAddr)
	{
		auto session = Create(socket, sockAddr);
		if (session)
			session->StartIo();
	}

	SessionPtr SessionManager::Get(uint64_t sessionId)
	{
		SharedLock lock(m_lock);

		auto it = m_sessions.find(sessionId);
		if (it == m_sessions.end())
			return nullptr;

		return it->second;
	}

	void SessionManager::Close()
	{
		std::vector<SessionPtr> sessions;

		{
			SharedLock lock(m_lock);
			sessions.reserve(m_sessions.size());
			for (auto& [id, session] : m_sessions)
			{
				if (session)
					sessions.emplace_back(session);
			}
		}

		for (auto& session : sessions)
		{
			if (session)
				session->Close();
		}
	}

	void SessionManager::Remove(uint64_t sessionId)
	{
		Lock lock(m_lock);

		auto it = m_sessions.find(sessionId);
		if (it == m_sessions.end())
			return;

		m_sessions.erase(it);
	}

}