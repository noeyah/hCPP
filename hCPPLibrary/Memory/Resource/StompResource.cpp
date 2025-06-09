#include "StompResource.h"
#include <memory_resource>
#include <cstring>
#include "Util/Macro.h"
#include "WinCommon.h"

namespace hlib::resource
{
	void* StompResource::do_allocate(size_t _Bytes, size_t _Align)
	{
		size_t align = std::max<size_t>(_Align, alignof(Header));
		size_t header_size = sizeof(Header);
		size_t offset = header_size + GUARD_SIZE;

		// ������ ���� �е�
		size_t padding = (align - (offset % align)) % align;

		// [ ��� | ���� | (�е�) ���� ������ | ���� ]

		size_t totalSize = offset + padding + _Bytes + GUARD_SIZE;
		size_t totalAllocSize = CalcSize(totalSize, PAGE_SIZE);

		void* base_address = ::VirtualAlloc(nullptr, totalAllocSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		ASSERT_CRASH(base_address);

		uint8_t* base = static_cast<uint8_t*>(base_address);
		uint8_t* user_ptr = base + offset + padding;

		// ���
		Header* header = reinterpret_cast<Header*>(base);
		header->userSize = _Bytes;
		header->totalAllocSize = totalAllocSize;
		header->padding = padding;
		header->baseAddress = base_address;
		header->allocThreadId = std::this_thread::get_id();
		header->allocTime = std::chrono::system_clock::now();

		// ����
		std::memset(base + header_size, static_cast<int>(GUARD_PATTERN), GUARD_SIZE);
		std::memset(user_ptr + _Bytes, static_cast<int>(GUARD_PATTERN), GUARD_SIZE);

		return static_cast<void*>(user_ptr);
	}

	void StompResource::do_deallocate(void* _Ptr, size_t _Bytes, size_t _Align)
	{
		if (!_Ptr)
			return;

		// _Ptr = userPtr

		// �Ҵ� �⺻ �ּ�
		MEMORY_BASIC_INFORMATION mbi;
		auto ret = ::VirtualQuery(_Ptr, &mbi, sizeof(mbi));
		ASSERT_CRASH(ret != 0);

		// [ ��� | ���� | (�е�) ���� ������ | ���� ]

		// ������ ���
		uint8_t* baseAddress = static_cast<uint8_t*>(mbi.AllocationBase);
		Header* header = reinterpret_cast<Header*>(baseAddress);
		uint8_t* prefixGuard = baseAddress + sizeof(Header);
		uint8_t* userPtr = prefixGuard + GUARD_SIZE + header->padding;
		uint8_t* suffixGuard = userPtr + header->userSize;

		// ������ ����
		ASSERT_CRASH(userPtr == static_cast<uint8_t*>(_Ptr));
		ASSERT_CRASH(header->userSize == _Bytes);

		// ���� ����
		std::vector<std::byte> guardPattern(GUARD_SIZE);
		std::memset(guardPattern.data(), static_cast<int>(GUARD_PATTERN), GUARD_SIZE);

		ASSERT_CRASH(std::memcmp(prefixGuard, guardPattern.data(), GUARD_SIZE) == 0);
		ASSERT_CRASH(std::memcmp(suffixGuard, guardPattern.data(), GUARD_SIZE) == 0);

		// ���� ���� ������ ����
		::VirtualFree(baseAddress, 0, MEM_RELEASE);
	}
}