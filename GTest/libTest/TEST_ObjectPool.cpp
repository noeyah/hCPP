#include <gtest/gtest.h>
#include <atomic>

#include "Memory/ObjectPool.h"
#include <memory>

struct TestObj
{
	int id = 0;
	inline static std::atomic<int> constructionCnt = 0;
	inline static std::atomic<int> destructionCnt = 0;

	TestObj(int val) : id(val)
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

class TestObjFactory : public hlib::IObjectFactory<TestObj>
{
public:
	std::atomic<int> createCnt = 0;
	std::atomic<int> lastId = 0;

	virtual TestObj* Create() override
	{
		createCnt++;
		return new TestObj(++lastId);
	}
};

struct TestDefaultObj
{
	inline static std::atomic<int> count = 0;
	TestDefaultObj() { count++; }
	~TestDefaultObj() { count--; }
	TestDefaultObj(const TestDefaultObj&) = delete;
	TestDefaultObj& operator=(const TestDefaultObj&) = delete;
};

TEST(ObjectPoolTest, Construction)
{
	const size_t size = 3;
	TestObj::constructionCnt.store(0);
	TestObj::destructionCnt.store(0);

	auto factory = std::make_unique<TestObjFactory>();
	auto factoryPtr = factory.get();

	hlib::ObjectPool<TestObj> pool(std::move(factory), size);

	EXPECT_EQ(factoryPtr->createCnt.load(), size);
	EXPECT_EQ(TestObj::constructionCnt.load(), size);
}

TEST(ObjectPoolTest, Return)
{
	const size_t size = 1;
	TestObj::constructionCnt.store(0);
	TestObj::destructionCnt.store(0);

	auto factory = std::make_unique<TestObjFactory>();

	hlib::ObjectPool<TestObj> pool(std::move(factory), size);

	EXPECT_EQ(TestObj::constructionCnt.load(), size);
	EXPECT_EQ(TestObj::destructionCnt.load(), 0);

	{
		auto obj = pool.Get();
		ASSERT_NE(obj, nullptr);
		ASSERT_NE(obj->id, 0);
	}

	{
		auto obj = pool.Get();
		ASSERT_NE(obj, nullptr);
		ASSERT_NE(obj->id, 0);
	}

	EXPECT_EQ(TestObj::constructionCnt.load(), size);
	EXPECT_EQ(TestObj::destructionCnt.load(), 0);
}

TEST(ObjectPoolTest, NewObj)
{
	TestObj::constructionCnt.store(0);
	TestObj::destructionCnt.store(0);

	auto factory = std::make_unique<TestObjFactory>();
	auto factoryPtr = factory.get();
	hlib::ObjectPool<TestObj> pool(std::move(factory), 0);

	EXPECT_EQ(TestObj::constructionCnt.load(), 0);
	EXPECT_EQ(TestObj::destructionCnt.load(), 0);
	EXPECT_EQ(factoryPtr->createCnt.load(), 0);

	auto obj = pool.Get();
	ASSERT_NE(obj, nullptr);
	ASSERT_NE(obj->id, 0);

	EXPECT_EQ(TestObj::constructionCnt.load(), 1);
	EXPECT_EQ(factoryPtr->createCnt.load(), 1);
}

TEST(ObjectPoolTest, Destruction)
{
	const size_t size = 3;
	TestDefaultObj::count.store(0);

	{
		hlib::ObjectPool<TestDefaultObj> pool(nullptr, size);
		EXPECT_EQ(TestDefaultObj::count.load(), size);
	}

	EXPECT_EQ(TestDefaultObj::count.load(), 0);
}

TEST(ObjectPoolTest, DefaultObj)
{
	TestDefaultObj::count.store(0);

	hlib::ObjectPool<TestDefaultObj> pool(nullptr, 1);
	EXPECT_EQ(TestDefaultObj::count.load(), 1);

	auto obj_1 = pool.Get();
	ASSERT_NE(obj_1, nullptr);

	auto obj_2 = pool.Get();
	ASSERT_NE(obj_2, nullptr);

	EXPECT_EQ(TestDefaultObj::count.load(), 2);
}

TEST(ObjectPoolTest, Death)
{
	TestObj::constructionCnt.store(0);
	TestObj::destructionCnt.store(0);

	hlib::ObjectPool<TestObj> pool(nullptr, 0);

	ASSERT_DEATH(pool.Get(), ".*objPtr.*");
	EXPECT_EQ(TestObj::constructionCnt.load(), 0);
	EXPECT_EQ(TestObj::destructionCnt.load(), 0);
}