#include <gtest/gtest.h>

#include "Memory/Container.h"
#include "Network/Buffer/PacketBuffer.h"
#include "Network/Buffer/AsyncSendQueue.h"

class AsyncSendQueueTest : public ::testing::Test
{
protected:
	hlib::net::AsyncSendQueue queue_;

	auto CreateBuffer(const std::string& desc)
	{
		if (desc.empty())
		{
			return std::make_shared<hlib::net::PacketBuffer>(1);
		}
		auto buffer = std::make_shared<hlib::net::PacketBuffer>(desc.size());
		buffer->Write(reinterpret_cast<const std::byte*>(desc.c_str()), desc.size());
		return buffer;
	}
};

TEST_F(AsyncSendQueueTest, Basic)
{
	EXPECT_TRUE(queue_.IsEmpty());
	EXPECT_EQ(queue_.GetQueueSize(), 0);
}

TEST_F(AsyncSendQueueTest, Push)
{
	auto buffer = CreateBuffer("t1e2s3t4");
	queue_.Push(buffer);

	EXPECT_FALSE(queue_.IsEmpty());
	EXPECT_EQ(queue_.GetQueueSize(), 1);
}

TEST_F(AsyncSendQueueTest, PushFail)
{
	std::shared_ptr<hlib::net::PacketBuffer> buffer1 = nullptr;
	queue_.Push(buffer1);
	EXPECT_EQ(queue_.GetQueueSize(), 0);

	auto buffer2 = CreateBuffer("");
	queue_.Push(buffer2);
	EXPECT_EQ(queue_.GetQueueSize(), 0);
}

TEST_F(AsyncSendQueueTest, Send)
{
	auto buf_1 = CreateBuffer("send data 1");
	size_t size_1 = buf_1->Size();
	auto buf_2 = CreateBuffer("send data 2");
	size_t size_2 = buf_2->Size();

	queue_.Push(buf_1);
	queue_.Push(buf_2);
	EXPECT_EQ(queue_.GetQueueSize(), 2);

	hlib::Vector<WSABUF> wsaBufs;
	size_t sendCnt = 0;
	size_t sendBytes = 0;
	queue_.PrepareSend(wsaBufs, 10, sendCnt, sendBytes);

	EXPECT_EQ(sendCnt, 2);
	EXPECT_EQ(wsaBufs.size(), 2);
	EXPECT_EQ(wsaBufs[0].len, size_1);
	EXPECT_EQ(wsaBufs[1].len, size_2);
	EXPECT_EQ(queue_.GetQueueSize(), 2);

	queue_.CompleteSend(1);
	EXPECT_EQ(queue_.GetQueueSize(), 1);

	queue_.PrepareSend(wsaBufs, 10, sendCnt, sendBytes);

	EXPECT_EQ(sendCnt, 1);
	EXPECT_EQ(wsaBufs.size(), 1);
	EXPECT_EQ(wsaBufs[0].len, size_2);

	queue_.CompleteSend(1);
	EXPECT_EQ(queue_.GetQueueSize(), 0);
	EXPECT_TRUE(queue_.IsEmpty());
}

TEST_F(AsyncSendQueueTest, PrepareMaxSize)
{
	auto buffer1 = CreateBuffer("a");
	queue_.Push(buffer1);
	auto buffer2 = CreateBuffer("bb");
	queue_.Push(buffer2);
	auto buffer3 = CreateBuffer("ccc");
	queue_.Push(buffer3);

	EXPECT_EQ(queue_.GetQueueSize(), 3);

	hlib::Vector<WSABUF> wsaBufs;
	size_t sendCnt = 0;
	size_t sendBytes = 0;
	queue_.PrepareSend(wsaBufs, 2, sendCnt, sendBytes);

	EXPECT_EQ(sendCnt, 2);
	EXPECT_EQ(wsaBufs.size(), 2);
	EXPECT_EQ(wsaBufs[0].len, 1);
	EXPECT_EQ(wsaBufs[1].len, 2);
	EXPECT_EQ(queue_.GetQueueSize(), 3);
}

TEST_F(AsyncSendQueueTest, OverComplete)
{
	auto buffer = CreateBuffer("one");
	queue_.Push(buffer);
	EXPECT_EQ(queue_.GetQueueSize(), 1);

	EXPECT_NO_THROW(queue_.CompleteSend(1));

	EXPECT_DEATH(queue_.CompleteSend(2), ".*sendSize <= queue_.size\\(\\).*");
}