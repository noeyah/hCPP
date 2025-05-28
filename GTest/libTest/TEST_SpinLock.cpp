#include <gtest/gtest.h>
#include <thread>
#include <atomic>
#include <cstdint>

#include "Lock//LockGuard.h"
#include "Lock//SpinLock.h"

using SpinLockGuard = hlib::Lock<hlib::SpinLock>;

TEST(SpinLockGuardTest, Basic) {
	hlib::SpinLock lock;

	ASSERT_NO_THROW({
		SpinLockGuard guard(lock);
		});

	ASSERT_NO_THROW({
		// guard 해제 확인
		SpinLockGuard guard2(lock);
		});
}

TEST(SpinLockGuardTest, Blocking) {
	hlib::SpinLock lock;
	std::uint32_t order = 0;

	std::thread t;

	{
		SpinLockGuard guard(lock);

		t = std::thread([&]() {
			SpinLockGuard guard1(lock);
			ASSERT_EQ(order, 1);
			order++;
			});

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		order++;
	} // 락해제

	t.join();

	ASSERT_EQ(order, 2);
}