#include <gtest/gtest.h>
#include <memory>
#include <atomic>

#include "Memory/Memory.h"
#include "Memory/Allocator.h"

struct MemoryTestData
{
	int id;
	double value;
	static inline std::atomic<int> destructor_count = 0;

	MemoryTestData(int i = 0, double v = 0.0) : id(i), value(v) {}
	~MemoryTestData()
	{
		destructor_count++;
	}
};

TEST(MemoryTest, Basic) 
{
	auto mem = hlib::Memory::Instance();

	const size_t size = 128;
	const size_t align = alignof(std::max_align_t);

	void* ptr = nullptr;
	ASSERT_NO_THROW({
		ptr = mem->Allocate(size, align);
		});

	// 할당 성공 체크
	ASSERT_NE(ptr, nullptr);

	// 간단히 쓰기/읽기 시도
	if (ptr) {
		*static_cast<char*>(ptr) = 'A';
		EXPECT_EQ(*static_cast<char*>(ptr), 'A');
	}

	ASSERT_NO_THROW({
		mem->Release(ptr, size, align);
		});
}

TEST(MemoryTest, MakeSharedPtr)
{
	MemoryTestData::destructor_count = 0;

	{
		std::shared_ptr<MemoryTestData> sp_data;

		ASSERT_NO_THROW({
			sp_data = hlib::MakeSharedPtr<MemoryTestData>(7777, 7979.5959);
			});
		ASSERT_NE(sp_data, nullptr);
		EXPECT_EQ(sp_data->id, 7777);
		EXPECT_DOUBLE_EQ(sp_data->value, 7979.5959);

		EXPECT_EQ(MemoryTestData::destructor_count.load(), 0);
	} // shared_ptr 삭제

	EXPECT_EQ(MemoryTestData::destructor_count.load(), 1);
}
