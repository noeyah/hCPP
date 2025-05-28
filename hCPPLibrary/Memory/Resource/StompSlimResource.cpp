#include "StompSlimResource.h"
#include "Util/Macro.h"
#include "WinCommon.h"

namespace hlib::resource
{
	void* StompSlimResource::do_allocate(size_t _Bytes, size_t _Align)
	{
		// PAGE_SIZE의 배수로 크기 계산
		const size_t totalAllocSize = CalcSize(_Bytes, PAGE_SIZE);

		// [          (유저 데이터)]
		const size_t offset = totalAllocSize - _Bytes;

		void* baseAddress = ::VirtualAlloc(nullptr, totalAllocSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		assert(baseAddress);

		uint8_t* userPtr = static_cast<uint8_t*>(baseAddress) + offset;
		return static_cast<void*>(userPtr);
	}

	void StompSlimResource::do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align)
	{
		const size_t totalAllocSize = CalcSize(_Bytes, PAGE_SIZE);
		const size_t offset = totalAllocSize - _Bytes;

		void* baseAddress = static_cast<uint8_t*>(_Ptr) - offset;
		auto ret = ::VirtualFree(baseAddress, 0, MEM_RELEASE);
		assert(ret != 0);
	}
}