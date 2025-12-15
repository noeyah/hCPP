#pragma once
#include <memory>
#include <atomic>
#include <thread>
#include <cstdint>

#include "WinCommon.h"
#include "Memory/Container.h"
#include "Thread/WorkerThread.h"

namespace hlib
{
	class IJobQueue;
	struct IoContext;

	class IocpDispatcher
	{
		HANDLE m_handle;
		WorkerThread m_thread;
		IJobQueue& m_jobQueue;

	public:
		IocpDispatcher(IJobQueue& queue);
		~IocpDispatcher();

		void Start(size_t thread_count);
		void Stop();

		HANDLE GetHandle() const { return m_handle; }

	private:
		void RequestStop();
		void Work();
		bool Dispatch();
		void ProcessIocpSuccess(IoContext* ioContext, DWORD bytesTransferred);
		
	};

}