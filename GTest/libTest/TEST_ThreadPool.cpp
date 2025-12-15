#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>

#include "Task/Job.h"
#include "Task/ThreadPool.h"
#include "Task/JobQueue.h"

class ThreadPoolTest : public ::testing::Test
{
protected:
	std::unique_ptr<hlib::JobQueue> jobQueue_;
	std::unique_ptr<hlib::ThreadPool> threadPool_;

	void SetUp() override
	{
		jobQueue_ = std::make_unique<hlib::JobQueue>();
		threadPool_ = std::make_unique<hlib::ThreadPool>(*jobQueue_, 4);
	}

	void TearDown() override
	{
		if (threadPool_)
			threadPool_->Stop();
	}
};

TEST_F(ThreadPoolTest, Basic)
{
	ASSERT_NE(threadPool_, nullptr);

	std::atomic<bool> job_executed = false;
	const std::chrono::milliseconds wait_ms(20);

	auto lambda = [&job_executed]() {
		job_executed.store(true);
	};
	auto job_ptr = std::make_shared<hlib::Job<decltype(lambda)>>(std::move(lambda));

	ASSERT_NO_THROW(jobQueue_->Push(job_ptr));

	std::this_thread::sleep_for(wait_ms);
	EXPECT_TRUE(job_executed.load());
}
