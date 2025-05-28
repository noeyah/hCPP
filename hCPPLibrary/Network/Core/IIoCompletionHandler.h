#pragma once

namespace hlib::net
{
	struct IoContext;

	class IIoCompletionHandler
	{
	public:
		virtual ~IIoCompletionHandler() = default;

		virtual void CompletedAsync(IoContext* context, DWORD bytesTransferd) = 0;
		virtual void ErrorHandler(IoContext* context, int32_t wsaErrorCode) = 0;
	};
}