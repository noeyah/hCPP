#pragma once
#include <memory_resource>
#include <string>
#include "Config.h"
#include "Util/Singleton.h"
#include "Resource/StompResource.h"
#include "Resource/StompSlimResource.h"
#include "Resource/LogResourceWrapper.h"
#include "Log.h"

namespace hlib
{
	class Memory
	{
		DEFINE_SINGLETON(Memory);
	
	public:
		void* Allocate(size_t size, size_t align)
		{
			return resource_->allocate(size, align);
		}

		void Release(void* ptr, size_t size, size_t align)
		{
			resource_->deallocate(ptr, size, align);
		}

		template<typename T>
		std::pmr::polymorphic_allocator<T> GetAllocator()
		{
			return std::pmr::polymorphic_allocator<T>(resource_);
		}

		void Print()
		{
			if (MEMORY_RESOURCE_MODE == MemoryMode::Log)
			{
				auto rsc = reinterpret_cast<hlib::resource::LogResourceWrapper*>(resource_);
				if (rsc)
				{
					rsc->Print();
				}
			}
		}

	private:
		Memory()
		{
			std::string modeName;

			switch (MEMORY_RESOURCE_MODE)
			{
			case MemoryMode::Pool:
			{
				static std::pmr::pool_options POOL_OPTIONS{
					.max_blocks_per_chunk = MEMORY_POOL_MAX_BLOCKS_PER_CHUNK,
					.largest_required_pool_block = MEMORY_POOL_LARGEST_BLOCK 
				};
				static std::pmr::synchronized_pool_resource POOL_RESOURCE(POOL_OPTIONS);
				modeName = "Pool";
				std::pmr::set_default_resource(&POOL_RESOURCE);
			} break;
			case MemoryMode::Stomp:
			{
				static hlib::resource::StompResource STOMP_RESOURCE;
				modeName = "Stomp";
				std::pmr::set_default_resource(&STOMP_RESOURCE);
			} break;
			case MemoryMode::StompSlim:
			{
				static hlib::resource::StompSlimResource STOMP_SLIM_RESOURCE;
				modeName = "StompSlim";
				std::pmr::set_default_resource(&STOMP_SLIM_RESOURCE);
			} break;
			case MemoryMode::Log:
			{
				static std::pmr::memory_resource* RSC = std::pmr::get_default_resource();
				modeName = "Log default";
				static hlib::resource::LogResourceWrapper LOG_RESOURCE_WRAPPER(modeName, RSC);
				std::pmr::set_default_resource(&LOG_RESOURCE_WRAPPER);
			} break;
			default:
			{
				modeName = "Default(new/delete)";
			} break;
			}

			resource_ = std::pmr::get_default_resource();
			LOG_INFO("Memory Resource Mode : {}", modeName);
		}
		~Memory() = default;

	private:
		std::pmr::memory_resource* resource_ = nullptr;
	};
}