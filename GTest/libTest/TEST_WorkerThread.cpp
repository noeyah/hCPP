#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>
#include "Thread/WorkerThread.h"

TEST(WorkerThreadTest, Basic)
{
	std::atomic<bool> fn_start = false;
	std::atomic<bool> fn_end = false;
	std::atomic<int> fn_loop_count = 0;
	std::atomic<bool> wake_up = false;

	auto work_fn = [&](std::atomic<bool>& isRunning) {
		fn_start = true;
		while (isRunning.load())
		{
			fn_loop_count++;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		fn_end = true;
	};

	auto wake_up_fn = [&]() {
		wake_up = true;
	};

	hlib::WorkerThread thread(work_fn, wake_up_fn);

	ASSERT_EQ(thread.GetThreadCount(), 0);
	ASSERT_FALSE(thread.IsRunning());

	thread.Start(1);
	ASSERT_EQ(thread.GetThreadCount(), 1);
	ASSERT_TRUE(thread.IsRunning());

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	ASSERT_TRUE(fn_start.load());

	int curr_loop_count = fn_loop_count.load();
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	// fn loop cnt > curr count
	EXPECT_GT(fn_loop_count.load(), curr_loop_count);

	thread.Stop();
	EXPECT_FALSE(thread.IsRunning());
	EXPECT_TRUE(wake_up.load());
	EXPECT_EQ(thread.GetThreadCount(), 0);
	EXPECT_TRUE(fn_end.load());
}