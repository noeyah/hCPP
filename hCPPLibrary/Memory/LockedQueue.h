#pragma once
#include <mutex>
#include <optional>
#include <atomic>
#include <queue>
#include <condition_variable>
#include "Container.h"

namespace hlib
{
	template<typename T>
	class LockedQueue
	{
	public:
		void Push(T item)
		{
			{
				std::lock_guard lock(mtx_);
				queue_.push(std::move(item));
			}
			cv_.notify_one();
		}

		std::optional<T> Pop()
		{
			std::lock_guard lock(mtx_);

			if (queue_.empty())
				return std::nullopt;

			T item = std::move(queue_.front());
			queue_.pop();
			return item;
		}

		bool TryPop(OUT T& out_item)
		{
			std::lock_guard lock(mtx_);

			if (queue_.empty())
				return false;

			out_item = std::move(queue_.front());
			queue_.pop();
			return true;
		}

		bool WaitPop(OUT T& out_item)
		{
			std::unique_lock lock(mtx_);

			cv_.wait(lock, [this]() { 
				return stopRequested_.load() || !queue_.empty(); 
				});

			if (stopRequested_.load())
				return false;

			out_item = std::move(queue_.front());
			queue_.pop();
			return true;
		}

		void Clear()
		{
			std::queue<T> emptyQueue;

			std::lock_guard lock(mtx_);
			queue_.swap(emptyQueue);
		}

		void RequestStop()
		{
			stopRequested_.store(true);
			cv_.notify_all();
		}

	private:
		std::queue<T> queue_;
		std::mutex mtx_;
		std::condition_variable cv_;
		std::atomic_bool stopRequested_{ false };
	};
}