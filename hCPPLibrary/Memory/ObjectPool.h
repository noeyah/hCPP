#pragma once
#include <memory>

#include "Container.h"
#include "Util/Macro.h"
#include "Lock.h"

namespace hlib
{
	template<typename T>
	class IObjectFactory
	{
	public:
		virtual ~IObjectFactory() = default;
		virtual T* Create() = 0;
	};

	template<typename T>
	class ObjectPool
	{
	public:
		ObjectPool(std::unique_ptr<IObjectFactory<T>> factory, size_t initSize) : factory_(std::move(factory))
		{
			pool_.reserve(initSize);
			all_.reserve(initSize);
			for (size_t i = 0; i < initSize; i++)
			{
				T* objPtr = CreateNew();
				if (objPtr)
				{
					pool_.push_back(objPtr);
					all_.push_back(objPtr);
				}
			}
		}

		~ObjectPool()
		{
			for (T* objPtr : all_)
			{
				delete objPtr;
			}

			pool_.clear();
			all_.clear();
		}

		std::shared_ptr<T> Get()
		{
			T* objPtr = nullptr;

			{
				Lock lock(spin_);
				if (!pool_.empty())
				{
					objPtr = pool_.back();
					pool_.pop_back();
				}
			}

			if (!objPtr)
			{
				objPtr = CreateNew();
				ASSERT_CRASH(objPtr);

				Lock lock(spin_);
				all_.push_back(objPtr);
			}

			auto deleter = [this](T* obj) {this->Return(obj); };

			return std::shared_ptr<T>(objPtr, deleter);
		}

		// 복사/이동 불가
		ObjectPool(const ObjectPool&) = delete;
		ObjectPool& operator=(const ObjectPool&) = delete;
		ObjectPool(ObjectPool&&) = delete;
		ObjectPool& operator=(ObjectPool&&) = delete;

	private:
		T* CreateNew()
		{
			if (factory_)
			{
				return factory_->Create();
			}
			else
			{
				// T가 추상클래스가 아니고, 기본 생성자가 있는 경우만 기본 생성
				if constexpr (!std::is_abstract_v<T> && std::is_default_constructible_v<T>)
				{
					try
					{
						return new T();
					}
					catch (...)
					{
						return nullptr;
					}
				}
				else
				{
					return nullptr;
				}
			}
		}

		void Return(T* objPtr)
		{
			ASSERT_CRASH(objPtr);

			Lock lock(spin_);
			pool_.push_back(objPtr);
		}

	private:
		SpinLock spin_;

		Vector<T*> pool_;
		Vector<T*> all_;

		std::unique_ptr<IObjectFactory<T>> factory_;
	};

}