#pragma once

namespace hlib
{
	class ILock
	{
	public:
		virtual ~ILock() = default;

		virtual void lock() = 0;
		virtual void unlock() = 0;
	};

	class ISharedLock
	{
	public:
		virtual ~ISharedLock() = default;

		virtual void lock_shared() = 0;
		virtual void unlock_shared() = 0;

		virtual void lock() = 0;
		virtual void unlock() = 0;
	};
}
