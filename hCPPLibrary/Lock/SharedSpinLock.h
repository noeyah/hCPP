#pragma once
#include <cstdint>
#include <atomic>
#include <chrono>
#include "ILock.h"

namespace hlib
{
	class SharedSpinLock : public ISharedLock
	{
	private:
		enum : uint64_t
		{
			WRITE_THREAD_MASK			= 0xFFFF'FFFF'0000'0000ULL, // ���� 32��Ʈ ����ũ
			READ_COUNT_MASK				= 0x0000'0000'FFFF'FFFFULL, // ���� 32��Ʈ ����ũ
			EMPTY_FLAG					= 0x0000'0000'0000'0000ULL, // 64��Ʈ 0
		};

		inline static constexpr uint32_t WRITE_THREAD_SHIFT = 32;
		inline static constexpr uint32_t SPIN_COUNT = 2000;
		inline static constexpr std::chrono::milliseconds TIMEOUT_MS{ 200 };

	public:
		SharedSpinLock() = default;
		~SharedSpinLock() = default;

		// ����/�̵� �Ұ�
		SharedSpinLock(const SharedSpinLock&) = delete;
		SharedSpinLock& operator=(const SharedSpinLock&) = delete;
		SharedSpinLock(SharedSpinLock&&) = delete;
		SharedSpinLock& operator=(SharedSpinLock&&) = delete;

		virtual void lock() override;
		virtual void unlock() override;

		virtual void lock_shared() override;
		virtual void unlock_shared() override;

	private:
		bool TryLock();
		bool TrySharedLock();

		bool IsOwner() const;

	private:
		std::atomic_uint64_t flag_{ EMPTY_FLAG };
		std::atomic_uint32_t pendingWriter_ = 0;
	};
}

