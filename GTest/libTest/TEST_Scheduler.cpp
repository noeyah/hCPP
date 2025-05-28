#include <gtest/gtest.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <memory>
#include <utility>
#include "Task/ThreadPool.h"
#include "Task/Scheduler.h"
#include "Task/JobQueue.h"

class SchedulerTest : public ::testing::Test
{
protected:
	std::unique_ptr<hlib::task::JobQueue> jobQueue_;
	std::unique_ptr<hlib::task::ThreadPool> threadPool_;
	std::unique_ptr<hlib::task::Scheduler> scheduler_;

	void SetUp() override
	{
		jobQueue_ = std::make_unique<hlib::task::JobQueue>();
		threadPool_ = std::make_unique<hlib::task::ThreadPool>(*jobQueue_, 4);
		scheduler_ = std::make_unique<hlib::task::Scheduler>(*jobQueue_);
	}

	void TearDown() override
	{
		if (scheduler_)
			scheduler_->Stop();

		if (threadPool_)
			threadPool_->Stop();
	}
};

TEST_F(SchedulerTest, Basic)
{
	ASSERT_NE(jobQueue_, nullptr);
	ASSERT_NE(threadPool_, nullptr);
	ASSERT_NE(scheduler_, nullptr);

	std::atomic<bool> job_executed_1 = false;
	std::atomic<bool> job_executed_2 = false;
	const std::chrono::milliseconds delay_1(10);
	const std::chrono::milliseconds delay_2(300);
	const std::chrono::milliseconds wait_ms(30);

	auto lambda_1 = [&job_executed_1]() {
		job_executed_1.store(true);
		};

	auto lambda_2 = [&job_executed_2]() {
		job_executed_2.store(true);
		};

	ASSERT_NO_THROW(scheduler_->PushAfter(delay_2, lambda_2));
	ASSERT_NO_THROW(scheduler_->PushAfter(delay_1, lambda_1));

	std::this_thread::sleep_for(wait_ms);
	EXPECT_TRUE(job_executed_1.load());
	EXPECT_FALSE(job_executed_2.load());
}
