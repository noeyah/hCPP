#pragma once
#include <memory>

#include "Container.h"
#include "Lock.h"

namespace hlib
{
	template<typename T>
	class ObjectPool
	{
		SpinLock spin_;
		Vector<std::shared_ptr<T>> pool_;

	public:
		ObjectPool() = default;

		template<typename... Args>
		std::shared_ptr<T> Get(Args&&... args)
		{
			Lock lock(spin_);

			if (pool_.empty())
			{
				return std::make_shared<T>(std::forward<Args>(args)...);
			}

			auto pObj = std::move(pool_.back());
			pool_.pop_back();

			return pObj;
		}

		void Return(std::shared_ptr<T> pObj)
		{
			if (!pObj)
				return;

			Lock lock(spin_);
			pool_.emplace_back(std::move(pObj));
		}

		// 복사/이동 불가
		ObjectPool(const ObjectPool&) = delete;
		ObjectPool& operator=(const ObjectPool&) = delete;
		ObjectPool(ObjectPool&&) = delete;
		ObjectPool& operator=(ObjectPool&&) = delete;
	};

}