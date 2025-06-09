#pragma once
#include <random>
#include <vector>
#include <cassert>
#include "Macro.h"

namespace hlib
{
	class Random
	{
	public:
		template <typename T>
		struct WeightItem
		{
			T value;
			int weight;

			WeightItem(T v, int w) : value(std::move(v)), weight(w) {}
		};

		template <typename T>
		static T GetRandomWeight(const std::vector<WeightItem<T>>& items);

		static bool RandomPercent(int percent)
		{
			std::uniform_int_distribution dis(1, 100);
			return dis(tls_gen_) <= percent;
		}

		static int RandomRange(int min, int max)
		{
			ASSERT_CRASH(min < max);
			std::uniform_int_distribution dis(min, max);
			return dis(tls_gen_);
		}

	private:
		Random() = delete;
		~Random() = delete;

	private:
		inline static thread_local std::mt19937 tls_gen_{ std::random_device{}() };
	};


	template<typename T>
	inline T Random::GetRandomWeight(const std::vector<WeightItem<T>>& items)
	{
		ASSERT_CRASH(!items.empty());

		int totalWeight = 0;
		for (const auto& item : items)
		{
			ASSERT_CRASH(item.weight > 0);
			totalWeight += item.weight;
		}

		std::uniform_int_distribution dis(1, totalWeight);
		int rand = dis(tls_gen_);

		int cumulativeWeight = 0;
		for (const auto& item : items)
		{
			cumulativeWeight += item.weight;
			if (rand <= cumulativeWeight)
			{
				return item.value;
			}
		}

		// 여기 오면 문제 있음
		CRASH("RandomUtil::GetRandomWeight");
		return items.back().value;
	}
}
