#pragma once
#include <memory>
#include <atomic>
#include <thread>
#include <cstdint>

#include "WinCommon.h"
#include "Memory/Container.h"
#include "Thread/WorkerThread.h"

namespace hlib::task
{
	class IJobQueue;
}

namespace hlib::net
{
	struct IoContext;

	class IocpDispatcher
	{
	public:
		IocpDispatcher(hlib::task::IJobQueue& queue);
		~IocpDispatcher();

		void Start(size_t thread_count);
		void Stop();

		HANDLE GetHandle() const { return iocpHandle_; }

	private:
		void RequestStop();
		void Work();
		bool Dispatch();
		void ProcessIocpSuccess(IoContext* ioContext, DWORD bytesTransferd);

	private:
		HANDLE iocpHandle_;
		WorkerThread thread_;
		hlib::task::IJobQueue& jobQueue_;
	};

}