#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "Network/Buffer/PacketBuffer.h"

TEST(PacketBufferTest, Construction)
{
	const size_t size = 1024;
	hlib::net::PacketBuffer buffer(size);

	EXPECT_EQ(buffer.Capacity(), size);
	EXPECT_EQ(buffer.Size(), 0);
	EXPECT_TRUE(buffer.Empty());
	EXPECT_NE(buffer.Data(), nullptr);
}

TEST(PacketBufferTest, Write)
{
	hlib::net::PacketBuffer buffer(100);
	std::string test_str = "Hello!";

	buffer.Write(reinterpret_cast<const std::byte*>(test_str.c_str()), test_str.size());

	EXPECT_EQ(buffer.Size(), test_str.size());
	EXPECT_FALSE(buffer.Empty());
	EXPECT_EQ(memcmp(buffer.Data(), test_str.c_str(), test_str.size()), 0);
}

TEST(PacketBufferTest, CommitWrite)
{
	hlib::net::PacketBuffer buffer(100);
	std::string test_str = "Commit!";

	auto writePtr = buffer.WritePtr();
	ASSERT_NE(writePtr, nullptr);

	memcpy(writePtr, test_str.c_str(), test_str.size());
	buffer.CommitWrite(test_str.size());

	EXPECT_EQ(buffer.Size(), test_str.size());
	EXPECT_EQ(memcmp(buffer.Data(), test_str.c_str(), test_str.size()), 0);
}

TEST(PacketBufferTest, WriteOverflow)
{
	hlib::net::PacketBuffer buffer(10);
	std::vector<std::byte> test_data(20);

	// Write 함수 내 assert 테스트
	ASSERT_DEATH(buffer.Write(test_data.data(), test_data.size()), ".*capacity_ >= usedSize_ \\+ size.*");
}