#pragma once

namespace hlib
{
	struct IoContext;

	class IoHandler
	{
	public:
		virtual ~IoHandler() = default;

		virtual void CompletedAsync(IoContext* context, DWORD bytesTransferred) = 0;
		virtual void ErrorHandler(IoContext* context, int32_t wsaErrorCode) = 0;
	};
}