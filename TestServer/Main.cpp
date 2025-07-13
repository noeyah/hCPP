#include <WinCommon.h>
#include <Memory/Memory.h>
#include <Command/CmdService.h>
#include <Network/NetConfig.h>
#include "Packet/PacketRegister.h"
#include "MainServer.h"
#include <Network/Connection/Session.h>
#include <Memory/Config.h>

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	hlib::Memory::Instance();
	PacketRegister::Initialize();

	hlib::net::NetServerConfig config;
	config.name = "main server";
	config.ip = "127.0.0.1";
	config.port = 7777;
	config.acceptCount = 30;
	config.socketPoolInitCount = 100;
	config.ioThreadCount = 2;

	MainServer server(config);
	server.Start();

	hlib::cmd::CmdService cmdService;
	cmdService.RegisterCommand("exit", "exit server",
							   [&server, &cmdService](const hlib::cmd::CommandArgs& args) {
								   server.Stop();
								   cmdService.Exit();
							   });

#ifdef _DEBUG

	if (hlib::MEMORY_RESOURCE_MODE == hlib::MemoryMode::Log)
	{
		cmdService.RegisterCommand("memory", "if memory mode is log, show alloc info",
								   [](const hlib::cmd::CommandArgs& args) {
									   hlib::Memory::Instance().Print();
								   });
	}

	cmdService.RegisterCommand("session", "show session count",
							   [](const hlib::cmd::CommandArgs& args) {
								   hlib::net::Session::Print();
							   });
#endif // _DEBUG

	cmdService.Run();
}
