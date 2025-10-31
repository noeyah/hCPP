#pragma once
#include <atomic>
#include <cstdint>
#include <memory>
#include <functional>
#include <unordered_map>

#include "WinCommon.h"
#include "Session.h"
#include "Network/NetCommon.h"
#include "Lock.h"

namespace hlib::net
{
	class SessionManager
	{
	public:
		using SessionFactory = std::function<Session::SharedPtr()>;

	public:
		SessionManager(HANDLE handle, SessionFactory factory);
		~SessionManager();
		void SetReturnSockCallback(Session::ReturnSockCallback sockReturnCallback);

		Session::SharedPtr Create(SOCKET socket, const SOCKADDR_IN* sockAddr);
		void CreateAndStart(SOCKET socket, const SOCKADDR_IN* sockAddr = nullptr);

		Session::SharedPtr Get(SessionId sessionId);

		void Close();

	private:
		void Remove(uint64_t sessionId);
		uint64_t GenerateSessionId() { return ++lastSessionId_; }

	private:
		SharedSpinLock sspin_;

		std::atomic_uint64_t lastSessionId_ = 0;
		std::unordered_map<SessionId, Session::SharedPtr> sessions_{};
		SessionFactory sessionFactory_{};

		HANDLE iocpHandle_;

		Session::ReturnSockCallback sockReturnCallback_{};
	};
}