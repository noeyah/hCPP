#include <gtest/gtest.h>
#include <atomic>

#include "Memory/ObjectPool.h"
#include <memory>

struct TestObj
{
	inline static std::atomic<int> constructionCnt = 0;
	inline static std::atomic<int> destructionCnt = 0;

	TestObj()
	{
		constructionCnt++;
	}

	~TestObj()
	{
		destructionCnt++;
	}

	TestObj(const TestObj&) = delete;
	TestObj& operator=(const TestObj&) = delete;
};

TEST(ObjectPoolTest, Return)
{
	TestObj::constructionCnt.store(0);
	TestObj::destructionCnt.store(0);

	hlib::ObjectPool<TestObj> pool;

	{
		auto obj = pool.Get();
		ASSERT_NE(obj, nullptr);
		pool.Return(obj);
	}
	
	{
		auto obj = pool.Get();
		ASSERT_NE(obj, nullptr);
		pool.Return(obj);
	}

	EXPECT_EQ(TestObj::constructionCnt.load(), 1);
	EXPECT_EQ(TestObj::destructionCnt.load(), 0);
}

TEST(ObjectPoolTest, NewObj)
{
	TestObj::constructionCnt.store(0);
	TestObj::destructionCnt.store(0);

	hlib::ObjectPool<TestObj> pool;

	EXPECT_EQ(TestObj::constructionCnt.load(), 0);
	EXPECT_EQ(TestObj::destructionCnt.load(), 0);

	auto obj = pool.Get();
	ASSERT_NE(obj, nullptr);
	pool.Return(obj);

	EXPECT_EQ(TestObj::constructionCnt.load(), 1);
}

TEST(ObjectPoolTest, Destruction)
{
	TestObj::constructionCnt.store(0);
	TestObj::destructionCnt.store(0);

	{
		hlib::ObjectPool<TestObj> pool;
		auto obj = pool.Get();
		EXPECT_EQ(TestObj::constructionCnt.load(), 1);
		pool.Return(obj);
	}

	EXPECT_EQ(TestObj::destructionCnt.load(), 1);
}

