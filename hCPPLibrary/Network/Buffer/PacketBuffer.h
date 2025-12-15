#pragma once
#include <memory>
#include <cassert>
#include "Memory/Buffer.h"
#include "Util/Macro.h"

namespace hlib
{
	class PacketBuffer
	{
		std::byte* m_pBuffer{};
		size_t m_readPos{};
		size_t m_writePos{};
		size_t m_capacity{};
		bool m_bOwner{ true };

	public:
		PacketBuffer(size_t size)
		{
			BufferSize eBufferSize = GetEnumBufferSize(size);

			if (eBufferSize == BufferSize::Dynamic)
				m_capacity = size;
			else
				m_capacity = GetByteSize(eBufferSize);

			m_pBuffer = Buffer::Acquire(m_capacity);
		}

		~PacketBuffer()
		{
			if (m_bOwner && m_pBuffer)
			{
				Buffer::Release(m_pBuffer, m_capacity);
				m_pBuffer = nullptr;
			}
		}

		// 이동
		PacketBuffer(PacketBuffer&& other) noexcept
		{
			m_pBuffer = other.m_pBuffer;
			m_readPos = other.m_readPos;
			m_writePos = other.m_writePos;
			m_capacity = other.m_capacity;
			m_bOwner = other.m_bOwner;

			other.m_pBuffer = nullptr;
			other.m_bOwner = false;
		}

		// 복사 X
		PacketBuffer(const PacketBuffer&) = delete;
		PacketBuffer& operator=(const PacketBuffer&) = delete;

		std::byte* WritePos() const { return m_pBuffer + m_writePos; }
		std::byte* ReadPos() const { return m_pBuffer + m_readPos; }
		size_t FreeSize() const { return m_capacity - m_writePos; }
		size_t DataSize() const { return m_writePos - m_readPos; }
		size_t Capacity() const { return m_capacity; }
		bool Empty() const { return m_readPos == m_writePos; }

		bool OnWrite(size_t size)
		{
			if (m_writePos + size > m_capacity)
				return false;

			m_writePos += size;
			return true;
		}

		bool OnRead(size_t size)
		{
			if (m_readPos + size > m_writePos)
				return false;

			m_readPos += size;

			if (m_readPos == m_writePos)
			{
				m_readPos = 0;
				m_writePos = 0;
			}
			return true;
		}

		void Clean()
		{
			if (m_readPos == 0)
				return;

			size_t dataSize = DataSize();
			if (dataSize > 0)
			{
				::memmove(m_pBuffer, ReadPos(), dataSize);
			}

			m_readPos = 0;
			m_writePos = dataSize;
		}

		void Clear()
		{
			m_readPos = 0;
			m_writePos = 0;
		}
	};

}