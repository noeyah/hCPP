#pragma once
#include <cstddef>
#include <memory_resource>

namespace hlib::resource
{
	class StompSlimResource : public std::pmr::memory_resource
	{
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
	};

}