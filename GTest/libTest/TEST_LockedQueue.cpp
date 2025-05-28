#include <gtest/gtest.h>
#include <optional>

#include "Memory/LockedQueue.h"

class LockedQueueTest : public ::testing::Test
{
protected:
	hlib::LockedQueue<int> queue_;
};

TEST_F(LockedQueueTest, Empty)
{
	int val;
	EXPECT_FALSE(queue_.TryPop(val));
	EXPECT_EQ(queue_.Pop(), std::nullopt);
}

TEST_F(LockedQueueTest, PushPop)
{
	queue_.Push(5252);

	int val;
	ASSERT_TRUE(queue_.TryPop(val));
	EXPECT_EQ(val, 5252);
	EXPECT_FALSE(queue_.TryPop(val));
}

TEST_F(LockedQueueTest, FIFO)
{
	queue_.Push(12);
	queue_.Push(34);
	queue_.Push(56);

	int val;
	ASSERT_TRUE(queue_.TryPop(val));
	EXPECT_EQ(val, 12);

	ASSERT_TRUE(queue_.TryPop(val));
	EXPECT_EQ(val, 34);

	ASSERT_TRUE(queue_.TryPop(val));
	EXPECT_EQ(val, 56);
}

TEST_F(LockedQueueTest, WaitPop)
{
	std::atomic<bool> recved = false;
	int recvVal = 0;

	std::thread t([&]() {
		int val;
		if (queue_.WaitPop(val))
		{
			recvVal = val;
			recved = true;
		}
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	EXPECT_FALSE(recved.load());

	queue_.Push(8282);

	t.join();

	EXPECT_TRUE(recved.load());
	EXPECT_EQ(recvVal, 8282);
}

TEST_F(LockedQueueTest, RequestStop)
{
	std::atomic<bool> wakeup = false;
	int recvVal = 0;

	std::thread t([&]() {
		int val;
		if (!queue_.WaitPop(val))
		{
			wakeup = true;
		}
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	EXPECT_FALSE(wakeup.load());

	queue_.RequestStop();
	t.join();

	EXPECT_TRUE(wakeup.load());
}