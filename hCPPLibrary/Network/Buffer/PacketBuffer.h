#pragma once
#include <memory>
#include <cassert>
#include "Memory/Buffer.h"
#include "Util/Macro.h"

namespace hlib::net
{
	class PacketBuffer
	{
	public:
		PacketBuffer(size_t size) : capacity_(size), buffer_(Buffer::Get(size))
		{
			ASSERT_CRASH(capacity_ > 0);
		}

		void Write(const std::byte* src, size_t size)
		{
			ASSERT_CRASH(capacity_ >= usedSize_ + size);

			memcpy(WritePtr(), src, size);
			usedSize_ += size;
		}

		void CommitWrite(size_t size)
		{
			ASSERT_CRASH(capacity_ >= usedSize_ + size);
			usedSize_ += size;
		}

		std::byte* WritePtr() const { return buffer_.get() + usedSize_; }

		size_t Size() const { return usedSize_; }
		std::byte* Data() const { return buffer_.get(); }
		bool Empty() const { return usedSize_ == 0; }
		size_t Capacity() const { return capacity_; }

	private:
		std::shared_ptr<std::byte[]> buffer_;
		size_t usedSize_ = 0;
		size_t capacity_ = 0;
	};

}