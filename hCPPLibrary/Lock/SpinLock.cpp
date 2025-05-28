#include "SpinLock.h"
#include <thread>
#include "Util/Macro.h"

namespace hlib
{
	void SpinLock::lock()
	{
		const auto start = std::chrono::steady_clock::now();

		while (!TryLock())
		{
			if (std::chrono::steady_clock::now() - start >= TIMEOUT_MS)
				CRASH("failed to acquire spin lock due to TIMEOUT_MS.");

			std::this_thread::yield();
		}
	}

	void SpinLock::unlock()
	{
		isLocked_.store(false, std::memory_order_release);
	}

	bool SpinLock::TryLock()
	{
		for (int i = 0; i < SPIN_COUNT; i++)
		{
			bool expected = false;
			if (isLocked_.compare_exchange_strong(expected, true))
				return true;
		}

		return false;
	}
}
