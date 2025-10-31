#include <WinCommon.h>
#include "core.h"
#include "Packet/PacketRegister.h"
#include "MainServer.h"

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	core::Memory::Instance();
	PacketRegister::Initialize();

	core::NetServerConfig config;
	config.name = "main server";
	config.ip = "127.0.0.1";
	config.port = 7777;
	config.acceptCount = 30;
	config.socketPoolInitCount = 100;
	config.ioThreadCount = 2;

	MainServer server(config);
	server.Start();

	core::CmdService cmdService;
	cmdService.RegisterCommand("exit", "exit server",
							   [&server, &cmdService](const core::CommandArgs& args) {
								   server.Stop();
								   cmdService.Exit();
							   });

#ifdef _DEBUG

	if (core::MEMORY_RESOURCE_MODE == core::MemoryMode::Log)
	{
		cmdService.RegisterCommand("memory", "if memory mode is log, show alloc info",
								   [](const core::CommandArgs& args) {
									   core::Memory::Instance()->Print();
								   });
	}

	cmdService.RegisterCommand("session", "show session count",
							   [](const core::CommandArgs& args) {
								   core::Session::Print();
							   });
#endif // _DEBUG

	cmdService.Run();
}
