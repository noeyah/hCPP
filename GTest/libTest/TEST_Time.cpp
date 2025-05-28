#include <gtest/gtest.h>
#include <chrono>
#include <cstdint>

#include "Util/TimeUtil.h"

TEST(TimeTest, NowSteady)
{
	auto t_1 = hlib::Time::NowSteady();

	std::this_thread::sleep_for(std::chrono::microseconds(10));
	auto t_2 = hlib::Time::NowSteady();

	// t_1 <= t_2
	EXPECT_LE(t_1, t_2);
}

TEST(TimeTest, TimeAfter)
{
	auto base = hlib::Time::NowSteady();
	std::chrono::milliseconds ms(100);
	uint64_t uint = 200;

	auto after_ms = hlib::Time::TimePointAfter(base, ms);
	EXPECT_EQ(after_ms, base + ms);

	auto after_uint = hlib::Time::TimePointAfter(base, uint);
	EXPECT_EQ(after_uint, base + std::chrono::milliseconds(uint));
}

TEST(TimeTest, Expired)
{
	auto now = hlib::Time::NowSteady();
	std::chrono::milliseconds ms(100);
	uint64_t uint = 200;

	EXPECT_TRUE(hlib::Time::IsExpired(now));
	EXPECT_TRUE(hlib::Time::IsExpired(now - std::chrono::milliseconds(100)));
	EXPECT_FALSE(hlib::Time::IsExpired(now + std::chrono::milliseconds(100)));
	EXPECT_FALSE(hlib::Time::IsExpired(now, uint));
	EXPECT_FALSE(hlib::Time::IsExpired(now, ms));
}

TEST(TimeTest, NowSystem)
{
	auto t_1 = hlib::Time::NowSystem();

	std::this_thread::sleep_for(std::chrono::microseconds(10));
	auto t_2 = hlib::Time::NowSystem();

	// t_1 <= t_2
	EXPECT_LE(t_1, t_2);
}