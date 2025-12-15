#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>

#include "Network/Buffer/SendBuffer.h"
#include "Network/Buffer/PacketBuffer.h"
#include "Memory/Container.h"

TEST(SendBufferTest, Construction)
{
    hlib::SendBuffer sendBuffer;
    EXPECT_TRUE(sendBuffer.IsEmpty());
    EXPECT_EQ(sendBuffer.GetQueueSize(), 0);
}

TEST(SendBufferTest, PushPacketBuffer)
{
    hlib::SendBuffer sendBuffer;
    
    auto packetBuffer = std::make_shared<hlib::PacketBuffer>(100);
    
    std::string test_data = "not empty";
    memcpy(packetBuffer->WritePos(), test_data.data(), test_data.size());
    packetBuffer->OnWrite(test_data.size());

    sendBuffer.Push(packetBuffer);

    EXPECT_FALSE(sendBuffer.IsEmpty());
    EXPECT_EQ(sendBuffer.GetQueueSize(), 1);
}

TEST(SendBufferTest, PrepareAndComplete)
{
    hlib::SendBuffer sendBuffer;
    auto packetBuffer = std::make_shared<hlib::PacketBuffer>(100);
    
    std::string test_data = "Hello SendBuffer!";
    memcpy(packetBuffer->WritePos(), test_data.data(), test_data.size());
    packetBuffer->OnWrite(test_data.size());

    sendBuffer.Push(packetBuffer);

    if (sendBuffer.Begin())
    {
        hlib::Vector<WSABUF> wsaBufs;
        size_t sendCount = 0;
        size_t sendBytes = 0;

        sendBuffer.PrepareSend(wsaBufs, 1, sendCount, sendBytes);

        EXPECT_EQ(sendCount, 1);
        EXPECT_EQ(sendBytes, test_data.size());
        ASSERT_EQ(wsaBufs.size(), 1);
        EXPECT_EQ(wsaBufs[0].len, test_data.size());
        EXPECT_EQ(memcmp(wsaBufs[0].buf, reinterpret_cast<char*>(packetBuffer->ReadPos()), test_data.size()), 0);

        sendBuffer.CompleteSend(1);
        
        sendBuffer.End();
    }

    EXPECT_TRUE(sendBuffer.IsEmpty());
    EXPECT_EQ(sendBuffer.GetQueueSize(), 0);
}
