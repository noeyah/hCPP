#pragma once
#include <mutex>
#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "Thread/WorkerThread.h"
#include "Memory/Container.h"
#include "Util/Singleton.h"

namespace hlib
{
	using CommandArgs = Vector<std::string>;
	using CommandAction = std::function<void(const CommandArgs&)>;

	class CmdService
	{
		struct CommandInfo
		{
			std::string desc;
			CommandAction action;
		};

	public:
		CmdService();
		~CmdService() = default;

		void Run();
		void Exit();
		void RegisterCommand(std::string name, std::string desc, CommandAction action);

	private:
		void Work(std::atomic_bool& running);
		void ReadCommand(const std::string& line, OUT std::string& cmd, OUT CommandArgs& args);
		void Execute(const std::string& cmd, const CommandArgs& args);

		void Command_Help(const CommandArgs& args);

	private:
		WorkerThread thread_;
		std::unordered_map<std::string, CommandInfo> commands_;
	};
}