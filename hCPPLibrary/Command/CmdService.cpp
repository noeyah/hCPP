#include "CmdService.h"
#include <iostream>
#include <format>
#include <syncstream>
#include <string>
#include <sstream>
#include "Log.h"

namespace hlib::cmd
{
	CmdService::CmdService()
		: thread_([this](std::atomic_bool& running) {this->Work(running); }, nullptr)
	{
		RegisterCommand("help", "show available commands.", [this](const CommandArgs& args) { this->Command_Help(args); });
	}

	void CmdService::Run()
	{
		CommandArgs args;
		Command_Help(args);

		thread_.StartAsCurrentThread();
	}

	void CmdService::Exit()
	{
		thread_.Stop();
	}

	void CmdService::RegisterCommand(std::string name, std::string desc, CommandAction action)
	{
		if (commands_.contains(name))
		{
			LOG_ERROR("Command {} is already registered", name);
			return;
		}

		commands_.emplace(std::move(name), CommandInfo{ std::move(desc), std::move(action) });
	}

	void CmdService::Work(std::atomic_bool& running)
	{
		std::string input;

		while (running.load())
		{
			std::cout << "> " << std::endl;
			std::getline(std::cin, input);

			if (input.empty())
				continue;

			std::string cmd;
			CommandArgs args;
			ReadCommand(input, OUT cmd, OUT args);

			Execute(cmd, args);
		}
	}

	void CmdService::ReadCommand(const std::string& line, OUT std::string& cmd, OUT CommandArgs& args)
	{
		std::stringstream stream(line);
		std::string buffer;

		stream >> cmd;

		while (stream >> buffer)
		{
			args.emplace_back(buffer);
		}
	}

	void CmdService::Execute(const std::string& cmd, const CommandArgs& args)
	{
		auto it = commands_.find(cmd);
		if (it == commands_.end())
		{
			LOG_WARN("Unknown command : {}\n Type 'help' for available commands.", cmd);
			return;
		}

		try
		{
			it->second.action(args);
		}
		catch (...)
		{
			LOG_ERROR("Error executing command : {}", cmd);
		}
	}

	void CmdService::Command_Help(const CommandArgs& args)
	{
		std::osyncstream stream(std::cout);

		stream << "========================================" << std::endl;

		for (const auto& [cmd, info] : commands_)
		{
			stream << std::format("{:<10} : {}\n", cmd, info.desc);
		}

		stream << "========================================" << std::endl;
	}

	
}