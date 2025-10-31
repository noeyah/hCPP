#pragma once
#include <mutex>
#include <atomic>
#include <memory>
#include <vector>
#include "Thread/WorkerThread.h"
#include "Util/Singleton.h"
#include "Memory/LockedQueue.h"
#include "LogMessage.h"
#include "ILogStream.h"
#include "ConsoleLog.h"
#include "FileLog.h"

namespace hlib::log
{
	class Logger : public Singleton<Logger>
	{
	public:
		Logger() : thread_([this](std::atomic_bool& running) { this->Work(running); },
						   [this]() { this->logQueue_.RequestStop(); })
		{
			AddStream(std::make_shared<ConsoleLog>());
			//AddStream(std::make_shared<FileLog>());

			thread_.Start();
		}

		~Logger()
		{
			logQueue_.RequestStop();
			thread_.Stop();
		}

		template <typename... Args>
		void Log(LogLevel level, const std::source_location& sl, std::format_string<Args...> fmt, Args&&... args)
		{
			std::string msg = std::format(fmt, std::forward<Args>(args)...);

			logQueue_.Push(LogMessage(level, std::move(msg), sl));
		}

		void AddStream(std::shared_ptr<ILogStream> stream)
		{
			std::lock_guard lock(mtx_);
			logStreams_.push_back(stream);
		}

		void ClearStream()
		{
			std::lock_guard lock(mtx_);
			logStreams_.clear();
		}

	private:
		void Work(std::atomic_bool& running)
		{
			while (running.load())
			{
				LogMessage log;
				if (!logQueue_.WaitPop(OUT log))
					break;

				{
					std::lock_guard lock(mtx_);
					if (logStreams_.empty())
						continue;

					for (const auto& stream : logStreams_)
					{
						stream->Write(log);
					}
				}
			}
		}

	private:
		LockedQueue<LogMessage> logQueue_;
		WorkerThread thread_;

		std::vector<std::shared_ptr<ILogStream>> logStreams_;
		std::mutex mtx_;
	};
}