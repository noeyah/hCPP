#pragma once
#include <memory_resource>
#include <string>
#include <atomic>
#include "Log.h"

namespace hlib::resource
{
	class LogResourceWrapper : public std::pmr::memory_resource
	{
	public:
		LogResourceWrapper(std::string name, std::pmr::memory_resource* rsc);
		virtual ~LogResourceWrapper();

		void Print();

	protected:
		virtual void* do_allocate(size_t _Bytes, size_t _Align) override;
		virtual void do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align) override;
		virtual bool do_is_equal(const memory_resource& _That) const noexcept override;

	private:
		std::pmr::memory_resource* rsc_;
		std::string name_;

		std::atomic_int64_t alloc_ = 0;
		std::atomic_int64_t dealloc_ = 0;
		std::atomic_int64_t bytesAlloc_ = 0;
		std::atomic_int64_t bytesDealloc_ = 0;
	};
}