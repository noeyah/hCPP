#pragma once
#include "Network/NetCommon.h"
#include "Memory/Container.h"

namespace hlib::net
{
	class BufferReader
	{
	private:
		inline static constexpr size_t BUFFER_SPARE = 3;

	public:
		BufferReader();
		~BufferReader() = default;

		bool Write(const void* data, size_t writeSize);
		bool CommitRead(size_t readSize);
		void Cleanup();

		void Clear()
		{
			buffer_.clear();
			writeIdx_ = 0;
			readIdx_ = 0;
		}

		size_t GetDataSize() const { return writeIdx_ - readIdx_; }
		size_t GetRemainSize() const { return buffer_.size() - writeIdx_; }
		
		std::byte* GetReadBuffer() { return buffer_.data() + readIdx_; }

	private:
		Vector<std::byte> buffer_{};
		size_t writeIdx_ = 0;
		size_t readIdx_ = 0;
	};
}