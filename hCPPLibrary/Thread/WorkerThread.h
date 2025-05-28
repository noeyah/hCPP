#pragma once
#include <thread>
#include <atomic>
#include <functional>
#include <mutex>
#include <vector>

namespace hlib
{
	class WorkerThread
	{
	public:
		WorkerThread(std::function<void(std::atomic_bool&)> workFunc, 
					 std::function<void()> notifyFunc);
		~WorkerThread();

		void Start(size_t count = 1);
		void StartAsCurrentThread();
		void Stop();
		
		size_t GetThreadCount() const { return workerCount_.load(); }
		bool IsRunning() const { return isRunning_.load(); }

	private:
		void Join();

		// 복사/이동 불가
		WorkerThread(const WorkerThread&) = delete;
		WorkerThread& operator=(const WorkerThread&) = delete;
		WorkerThread(WorkerThread&&) = delete;
		WorkerThread& operator=(WorkerThread&&) = delete;

	private:
		std::vector<std::thread> threads_;
		std::mutex mtx_;
		std::atomic_bool isRunning_ = false;
		std::atomic_size_t workerCount_ = 0;

		std::function<void(std::atomic_bool&)> workFunc_;
		std::function<void()> notifyFunc_;
	};

}