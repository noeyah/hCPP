#include "BufferReader.h"
#include "Log.h"

namespace hlib::net
{
	BufferReader::BufferReader() : buffer_(MAX_BUFFER_SIZE* BUFFER_SPARE)
	{
	}

	bool BufferReader::Write(const void* data, size_t writeSize)
	{
		if (GetRemainSize() < writeSize)
			return false;

		memcpy(buffer_.data() + writeIdx_, data, writeSize);
		writeIdx_ += writeSize;
		return true;
	}

	bool BufferReader::CommitRead(size_t readSize)
	{
		if (GetDataSize() < readSize)
			return false;

		readIdx_ += readSize;
		return true;
	}

	void BufferReader::Cleanup()
	{
		auto dataSize = GetDataSize();
		if (dataSize == 0)
		{
			writeIdx_ = 0;
			readIdx_ = 0;
			return;
		}

		if (GetRemainSize() < MAX_BUFFER_SIZE)
		{
			memmove(buffer_.data(), buffer_.data() + readIdx_, dataSize);
			readIdx_ = 0;
			writeIdx_ = dataSize;
		}
	}

}