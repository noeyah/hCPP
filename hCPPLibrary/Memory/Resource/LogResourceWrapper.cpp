#include "LogResourceWrapper.h"
#include "Log.h"

namespace hlib::resource
{
	LogResourceWrapper::LogResourceWrapper(std::string name, std::pmr::memory_resource* rsc) 
		: name_(std::move(name)), rsc_(rsc)
	{
		if (!rsc_)
		{
			rsc_ = std::pmr::get_default_resource();
		}
		//LOG_DEBUG("LogResource {}({})", name_, typeid(*rsc_).name());
	}

	LogResourceWrapper::~LogResourceWrapper()
	{
		//LOG_DEBUG("~LogResource {} --- {}({}) : {}({})", name_, alloc_.load(), bytesAlloc_.load(), dealloc_.load(), bytesDealloc_.load());
	}

	void LogResourceWrapper::Print()
	{
		LOG_DEBUG("[LogResourceWrapper] {}({}) : {}({})", alloc_.load(), bytesAlloc_.load(), dealloc_.load(), bytesDealloc_.load());
	}

	void* LogResourceWrapper::do_allocate(size_t _Bytes, size_t _Align)
	{
		alloc_++;
		bytesAlloc_ += _Bytes;
		void* ptr = rsc_->allocate(_Bytes, _Align);

		//LOG_DEBUG("{} do_allocate : bytes {}, align {}, --- {} : {}", name_, _Bytes, _Align, bytesAlloc_.load(), bytesDealloc_.load());
		return ptr;
	}

	void LogResourceWrapper::do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align)
	{
		dealloc_++;
		bytesDealloc_ += _Bytes;

		rsc_->deallocate(_Ptr, _Bytes, _Align);
		//LOG_DEBUG("{} do_deallocate : bytes {}, align {} --- {} : {}", name_, _Bytes, _Align, bytesAlloc_.load(), bytesDealloc_.load());
	}

	bool LogResourceWrapper::do_is_equal(const memory_resource& _That) const noexcept
	{
		return this == &_That;
	}
}

