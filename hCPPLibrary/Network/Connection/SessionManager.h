#pragma once
#include <atomic>
#include <cstdint>
#include <memory>
#include <functional>
#include <unordered_map>

#include "WinCommon.h"
#include "Network/NetCommon.h"
#include "Lock.h"

namespace hlib
{
	class Session;
	using SessionPtr = std::shared_ptr<Session>;

	class SessionManager
	{
		using SessionFactory = std::function<std::shared_ptr<Session>()>;

		HANDLE m_handle;
		SharedSpinLock m_lock;
		std::atomic_uint64_t m_lastID = 0;
		std::unordered_map<uint64_t, SessionPtr> m_sessions{};
		SessionFactory m_sessionFactory{};

	public:

		SessionManager(HANDLE handle) : m_handle(handle) {}
		~SessionManager();

		void SetSessionFactory(SessionFactory callback) { m_sessionFactory = std::move(callback); }

		SessionPtr Create(SOCKET socket, const SOCKADDR_IN* sockAddr);
		void CreateAndStart(SOCKET socket, const SOCKADDR_IN* sockAddr = nullptr);

		SessionPtr Get(uint64_t sessionId);

		void Close();

	private:
		void Remove(uint64_t sessionId);
		uint64_t GenerateID() { return ++m_lastID; }

	};
}