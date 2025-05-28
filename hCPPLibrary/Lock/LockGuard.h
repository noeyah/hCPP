#pragma once
#include <concepts>
#include <mutex>
#include <shared_mutex>
#include "ILock.h"

namespace hlib
{
	/*template<typename T>
	concept MutexConcept =
		std::same_as<T, std::mutex>
		|| std::same_as<T, std::shared_mutex>
		|| std::derived_from<T, ILock>
		|| std::derived_from<T, ISharedLock>;

	template<typename T>
	concept SharedMutexConcept =
		std::same_as<T, std::shared_mutex>
		|| std::derived_from<T, ISharedLock>;*/

	template<typename T>
	concept LockUnlockConcept = requires(T t)
	{
		t.lock();
		t.unlock();
	};

	template<typename T>
	concept SharedLockUnlockConcept = requires(T t)
	{
		t.lock_shared();
		t.unlock_shared();
	};

	template <LockUnlockConcept T>
	class Lock
	{
	public:
		explicit Lock(T& lock) : lock_(lock)
		{
			lock_.lock();
		}

		~Lock()
		{
			lock_.unlock();
		}

		// 복사/이동 삭제
		Lock(const Lock&) = delete;
		Lock& operator=(const Lock&) = delete;
		Lock(Lock&&) = delete;
		Lock& operator=(Lock&&) = delete;

	private:
		T& lock_;
	};

	template <SharedLockUnlockConcept T>
	class SharedLock
	{
	public:
		explicit SharedLock(T& lock) : lock_(lock)
		{
			lock_.lock_shared();
		}

		~SharedLock()
		{
			lock_.unlock_shared();
		}

		// 복사/이동 삭제
		SharedLock(const SharedLock&) = delete;
		SharedLock& operator=(const SharedLock&) = delete;
		SharedLock(SharedLock&&) = delete;
		SharedLock& operator=(SharedLock&&) = delete;

	private:
		T& lock_;
	};
}