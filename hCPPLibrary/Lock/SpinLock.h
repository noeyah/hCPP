#pragma once
#include <cstdint>
#include <atomic>
#include <chrono>
#include "ILock.h"

namespace hlib
{
	class SpinLock : public ILock
	{
	private:
		inline static constexpr uint32_t SPIN_COUNT = 2000;
		inline static constexpr std::chrono::milliseconds TIMEOUT_MS{ 200 };

	public:
		SpinLock() = default;
		~SpinLock() = default;

		// 복사/이동 불가
		SpinLock(const SpinLock&) = delete;
		SpinLock& operator=(const SpinLock&) = delete;
		SpinLock(SpinLock&&) = delete;
		SpinLock& operator=(SpinLock&&) = delete;

		virtual void lock() override;
		virtual void unlock() override;

	private:
		bool TryLock();

	private:
		std::atomic_bool isLocked_{ false };
	};
}

