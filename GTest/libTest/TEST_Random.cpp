#include <gtest/gtest.h>
#include <map>
#include <vector>
#include <string>
#include <format>

#include "Log.h"
#include "Util/RandomUtil.h"

TEST(RandomTest, RandomPercent)
{
	EXPECT_FALSE(hlib::Random::RandomPercent(0));
	EXPECT_TRUE(hlib::Random::RandomPercent(100));
}

TEST(RandomTest, RandomPercentNear)
{
	const int gachaCount = 100000;
	const int targetPercent = 30;
	int winCount = 0;

	for (int i = 0; i < gachaCount; i++)
	{
		if (hlib::Random::RandomPercent(targetPercent))
		{
			winCount++;
		}
	}

	double resultRatio = static_cast<double>(winCount) / gachaCount;
	double targetRatio = static_cast<double>(targetPercent) / 100.0;
	EXPECT_NEAR(resultRatio, targetRatio, 0.01);

	//LOG_INFO("[RandomPercentNear] gacha {}, percent {}({}) -> win {}({})", gachaCount, targetPercent, targetRatio, winCount, resultRatio);
}

TEST(RandomTest, RandomRange)
{
	const int min = 10;
	const int max = 15;
	const int gachaCount = 1000;
	bool out_of_range = false;
	std::map<int, int> counts;

	for (int i = 0; i < gachaCount; i++)
	{
		int val = hlib::Random::RandomRange(min, max);
		if (val < min || val > max)
		{
			out_of_range = true;
			break;
		}
		counts[val]++;
	}

	EXPECT_FALSE(out_of_range);
	EXPECT_EQ(counts.size(), max - min + 1);

	std::string result;
	for (const auto& pair : counts)
	{
		result += std::format("[{}]: {} ", pair.first, pair.second);
	}
	//LOG_INFO("[RandomRange] gacha {}, {} ~ {}\n-> {}", gachaCount, min, max, result);
}

TEST(RandomTest, Death)
{
	EXPECT_DEATH(hlib::Random::RandomRange(5, 5), "min < max");
	EXPECT_DEATH(hlib::Random::RandomRange(10, 5), "min < max");
}

TEST(RandomTest, Weight)
{
	std::vector<hlib::Random::WeightItem<std::string>> items = { {"hi", 10} };
	std::string result = hlib::Random::GetRandomWeight(items);
	EXPECT_EQ(result, "hi");
}

TEST(RandomTest, RandomWeight)
{
	std::vector<hlib::Random::WeightItem<std::string>> items = {
		{"aaa", 10}, {"bbb", 20}, {"ccc", 30} 
	};
	const int gachaCount = 100000;
	std::map<std::string, int> map;

	for (int i = 0; i < gachaCount; i++)
	{
		map[hlib::Random::GetRandomWeight(items)]++;
	}

	EXPECT_EQ(map.size(), items.size());

	double totalWeight = 0;
	for (const auto& item : items)
	{
		totalWeight += item.weight;
	}

	std::string log;
	for (const auto& item : items)
	{
		double targetRatio = static_cast<double>(item.weight) / totalWeight;
		double resultRatio = static_cast<double>(map[item.value]) / gachaCount;
		EXPECT_NEAR(resultRatio, targetRatio, 0.02);

		log += std::format("[{}({}/{})]: {}({}) ", item.value, item.weight, totalWeight, map[item.value], resultRatio);
	}

	//LOG_INFO("[RandomWeight] {}", log);
}
