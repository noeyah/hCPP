#pragma once
#include <cstddef>
#include <memory_resource>
#include <chrono>

namespace hlib::resource
{
	class StompResource : public std::pmr::memory_resource
	{
	private:
		struct Header
		{
			size_t userSize;									// 사용자가 요청한 실제 크기
			size_t totalAllocSize;								// VirtualAlloc으로 할당한 전체 크기
			size_t padding;										// 패딩 크기
			void* baseAddress;									// VirtualAlloc이 반환한 시작 주소
			std::thread::id allocThreadId;						// 할당 스레드 ID
			std::chrono::system_clock::time_point allocTime;	// 할당 시각
		};

	private:
		virtual void* do_allocate(size_t _Bytes, size_t _Align) override;
		virtual void do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align) override;
		virtual bool do_is_equal(const memory_resource& _That) const noexcept override
		{
			return this == &_That;
		}

	private:
		inline size_t CalcSize(size_t value, size_t align) const noexcept
		{
			return (value + align - 1) & ~(align - 1);
		}

	private:
		inline static constexpr size_t PAGE_SIZE = 0x1000;
		inline static constexpr size_t GUARD_SIZE = 32;
		static constexpr std::byte GUARD_PATTERN = std::byte{ 0xCD };
	};

}