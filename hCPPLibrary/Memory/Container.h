#pragma once
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>

#define OUT 

namespace hlib
{
	using String = std::pmr::string;

	template<typename T>
	using Vector = std::pmr::vector<T>;

	template<typename T>
	using Deque = std::pmr::deque<T>;

	template<typename T>
	using Queue = std::queue<T, Deque<T>>;

	template<typename T>
	using Stack = std::stack<T, Deque<T>>;

	template<typename T>
	using List = std::pmr::list<T>;

	template<typename K, typename V>
	using Map = std::pmr::map<K, V>;

	template<typename K, typename V>
	using UnorderedMap = std::pmr::unordered_map<K, V>;

	template<typename K, typename V>
	using MultiMap = std::pmr::multimap<K, V>;

	template<typename K, typename V>
	using UnorderedMultiMap = std::pmr::unordered_multimap<K, V>;

	template<typename T>
	using Set = std::pmr::set<T>;

	template<typename T>
	using UnorderedSet = std::pmr::unordered_set<T>;

	template<typename T>
	using PriorityQueue = std::priority_queue<T, Vector<T>>;
}