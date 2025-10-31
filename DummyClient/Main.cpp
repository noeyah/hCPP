#include "core.h"
#include "Server.h"
#include "Packet/PacketRegister.h"

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	core::Memory::Instance();
	PacketRegister::Initialize();


	core::NetClientConfig config;
	config.name = "dummy client";
	config.ip = "127.0.0.1";
	config.port = 7777;
	config.connectCount = 100;

	Server server(config);
	server.Start();

	core::CmdService cmdService;
	cmdService.RegisterCommand("exit", "exit server",
							   [&server, &cmdService](const core::CommandArgs& args) {
									server.Stop();
									cmdService.Exit();
								});
	cmdService.Run();
}
