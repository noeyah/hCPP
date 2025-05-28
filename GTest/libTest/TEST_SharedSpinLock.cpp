#include <gtest/gtest.h>
#include <thread>

#include "Lock//LockGuard.h"
#include "Lock//SharedSpinLock.h"

using namespace hlib;

TEST(SharedSpinLockTest, WriteLock)
{
	SharedSpinLock lock;

	ASSERT_NO_THROW({
		Lock guard(lock);
		});
}

TEST(SharedSpinLockTest, ReadLock)
{
	SharedSpinLock lock;

	ASSERT_NO_THROW({
		SharedLock guard(lock);
		});
}

TEST(SharedSpinLockTest, MultipleReader)
{
	SharedSpinLock lock;
	SharedLock reader1(lock);

	ASSERT_NO_THROW({
		SharedLock reader2(lock);
		});
}

TEST(SharedSpinLockTest, Block)
{
	SharedSpinLock lock;
	std::atomic<bool> reader_started_locking = false;
	std::atomic<bool> reader_finished = false;
	std::atomic<bool> write_lock_released = false;
	std::thread reader_thread;

	{
		// 쓰기 락 시작
		Lock writer_guard(lock);

		reader_thread = std::thread([&]() {
			reader_started_locking = true;

			SharedLock reader_guard(lock);
			EXPECT_TRUE(write_lock_released.load());
			reader_finished = true;
			});

		// 읽기 락 시도할 때까지 대기
		while (!reader_started_locking.load())
		{
			std::this_thread::yield();
		}

		// 읽기 락 블락 상태
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		ASSERT_FALSE(reader_finished.load());

		write_lock_released = true;
	} // 쓰기 락 해제

	reader_thread.join();
	ASSERT_TRUE(reader_finished.load());
}