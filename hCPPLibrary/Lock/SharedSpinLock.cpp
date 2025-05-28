#include "SharedSpinLock.h"
#include "Thread/ThreadLocalStorage.h"
#include "Util/Macro.h"

namespace hlib
{
	// write lock
	void SharedSpinLock::lock()
	{
		if (IsOwner())
		{
			TLS::writeRecursionCount++;
			return;
		}

		const auto start = std::chrono::steady_clock::now();
		pendingWriter_++;

		while (!TryLock())
		{
			if (std::chrono::steady_clock::now() - start >= TIMEOUT_MS)
			{
				CRASH("failed to acquire SharedSpinLock::lock due to TIMEOUT_MS.");
			}

			std::this_thread::yield();
		}

		TLS::writeRecursionCount = 1;
		pendingWriter_--;
	}

	void SharedSpinLock::unlock()
	{
		assert(IsOwner());

		if (--TLS::writeRecursionCount == 0)
		{
			flag_.store(EMPTY_FLAG, std::memory_order_release);
		}
	}

	// read lock
	void SharedSpinLock::lock_shared()
	{
		if (IsOwner())
		{
			TLS::readRecursionCount++;
			return;
		}

		const auto start = std::chrono::steady_clock::now();

		while (!TrySharedLock())
		{
			if (std::chrono::steady_clock::now() - start >= TIMEOUT_MS)
			{
				CRASH("failed to acquire SharedSpinLock::lock_shared due to TIMEOUT_MS.");
			}

			std::this_thread::yield();
		}
	}

	void SharedSpinLock::unlock_shared()
	{
		if (IsOwner())
		{
			TLS::readRecursionCount--;
			return;
		}

		uint64_t value = flag_.fetch_sub(1);
		assert((value & READ_COUNT_MASK) > 0);
	}

	bool SharedSpinLock::TryLock()
	{
		for (int i = 0; i < SPIN_COUNT; i++)
		{
			const uint64_t desired = (static_cast<uint64_t>(TLS::info.workerId) << WRITE_THREAD_SHIFT) & WRITE_THREAD_MASK;
			uint64_t expected = EMPTY_FLAG;
			if (flag_.compare_exchange_strong(expected, desired, std::memory_order_acquire, std::memory_order_relaxed))
			{
				return true;
			}
		}
		return false;
	}

	bool SharedSpinLock::TrySharedLock()
	{
		for (int i = 0; i < SPIN_COUNT; i++)
		{
			uint64_t expected = flag_.load();
			if ((pendingWriter_.load() == 0) && (expected & WRITE_THREAD_MASK) == 0)
			{
				uint64_t desired = expected + 1;
				if (flag_.compare_exchange_weak(expected, desired, std::memory_order_acquire, std::memory_order_relaxed))
				{
					return true;
				}
			}
		}

		return false;
	}

	bool SharedSpinLock::IsOwner() const
	{
		uint64_t currentFlag = flag_.load();
		uint32_t ownerId = static_cast<uint32_t>((currentFlag & WRITE_THREAD_MASK) >> WRITE_THREAD_SHIFT);
		return (ownerId != 0 && ownerId == TLS::info.workerId);
	}
	
}
//
//void hlib::SpinLock::lock()
//{
//	if (!TryLock())
//		EnterSpin();
//}
//
//void hlib::SpinLock::unlock()
//{
//	is_locked.store(false, std::memory_order_release);
//}
//
//bool hlib::SpinLock::TryLock()
//{
//	bool expected = false;
//	return is_locked.compare_exchange_strong(expected, true);
//}
//
//void hlib::SpinLock::EnterSpin()
//{
//	constexpr uint32_t spin_to_yield_count = SPIN_COUNT + YIELD_SPIN_COUNT;
//	constexpr uint32_t spin_to_sleep_count = SPIN_COUNT + YIELD_SPIN_COUNT + SLEEP_SPIN_COUNT;
//	const auto start_time = std::chrono::steady_clock::now();
//
//	for (uint32_t i = 0; ; i++)
//	{
//		if (i < SPIN_COUNT)
//		{
//			// need Thread.SpinWait ...
//			// just keep going ...
//		}
//		else if (i < spin_to_yield_count)
//		{
//			std::this_thread::yield();
//		}
//		else
//		{
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//			// 우선 순위가 낮은 스레드는 계속 밀려날 수 있음
//			// 일정 횟수가 지나면 다시 시도해봄
//			if (i >= spin_to_sleep_count)
//				i = -1;
//		}
//
//		if (TryLock())
//			return;
//
//		if (std::chrono::steady_clock::now() - start_time > TIMEOUT_MS)
//			CRASH("failed to acquire lock after MAX_SPIN_COUNT. deadlock or extreme contention is suspected.");
//	}
//}
