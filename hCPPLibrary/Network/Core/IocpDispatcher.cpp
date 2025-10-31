#include "IocpDispatcher.h"
#include <cassert>
#include <cstring>
#include "Task/IJobQueue.h"
#include "Task/TaskUtil.h"
#include "Memory/Allocator.h"
#include "Util/Macro.h"
#include "Log.h"
#include "Network/InternalNetwork.h"
#include "IoContext.h"
#include "IIoCompletionHandler.h"

namespace hlib::net
{
	IocpDispatcher::IocpDispatcher(hlib::task::IJobQueue& jobQueue) 
		: jobQueue_(jobQueue),
		thread_([this](std::atomic_bool& running) {this->Work(); },
				[this]() { this->RequestStop(); })
	{
		iocpHandle_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
		ASSERT_CRASH(iocpHandle_ != INVALID_HANDLE_VALUE);
	}

	IocpDispatcher::~IocpDispatcher()
	{
		if (iocpHandle_ != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(iocpHandle_);
		}
	}

	void IocpDispatcher::Start(size_t threadCount)
	{
		thread_.Start(threadCount);
		LOG_INFO("iocp dispatcher start");
	}

	void IocpDispatcher::Stop()
	{
		thread_.Stop();
		LOG_INFO("iocp dispatcher stop");
	}

	void IocpDispatcher::RequestStop()
	{
		auto count = thread_.GetThreadCount();
		for (size_t i = 0; i < count; i++)
		{
			PostQueuedCompletionStatus(iocpHandle_, 0, 0, NULL);
		}
	}

	void IocpDispatcher::Work()
	{
		while (true)
		{
			if (!Dispatch())
				break;
		}
	}

	bool IocpDispatcher::Dispatch()
	{
		DWORD bytesTransferred{};
		ULONG_PTR completionKey{};
		IoContext* ioContext{};

		auto ret = ::GetQueuedCompletionStatus(iocpHandle_, 
											   &bytesTransferred,
											   &completionKey, 
											   reinterpret_cast<LPOVERLAPPED*>(&ioContext), 
											   INFINITE);

		if (!ioContext)
		{
			if (!ret)
			{
				CRASH("GQCS returned error and io context is null - {}", ::WSAGetLastError());
				return false;
			}
			else if (bytesTransferred == 0 && completionKey == 0)
			{
				// stop 신호 -> loop 종료
				return false;
			}
			else
			{
				CRASH("GQCS returned success and io context is null");
				return false;
			}
		}

		auto ioHandler = ioContext->ioHandler;
		if (!ioHandler)
		{
			LOG_DEBUG("ioHandler is null, {}", static_cast<uint8_t>(ioContext->ioOperation));
			return true;
		}

		if (ret)
		{
			ProcessIocpSuccess(ioContext, bytesTransferred);
		}
		else
		{
			auto wsaErrorCode = WSAGetLastError();
			ioHandler->ErrorHandler(ioContext, wsaErrorCode);
		}

		return true;
	}

	void IocpDispatcher::ProcessIocpSuccess(IoContext* ioContext, DWORD bytesTransferred)
	{
		auto jobLambda = [ioContext, bytesTransferred]() mutable {
			if (ioContext && ioContext->ioHandler)
			{
				ioContext->ioHandler->CompletedAsync(ioContext, bytesTransferred);
			}
		};

		hlib::task::PushJob(jobQueue_, std::move(jobLambda));
	}
}