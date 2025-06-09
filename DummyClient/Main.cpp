#include <WinCommon.h>
#include <Memory/Memory.h>
#include <Network/NetConfig.h>
#include <Command/CmdService.h>
#include "Server.h"
#include "Packet/PacketRegister.h"

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	hlib::Memory::Instance();
	PacketRegister::Initialize();


	hlib::net::NetClientConfig config;
	config.name = "dummy client";
	config.ip = "127.0.0.1";
	config.port = 7777;
	config.connectCount = 200;

	Server server(config);
	server.Start();

	hlib::cmd::CmdService cmdService;
	cmdService.RegisterCommand("exit", "exit server",
							   [&server, &cmdService](const hlib::cmd::CommandArgs& args) {
									server.Stop();
									cmdService.Exit();
								});
	cmdService.Run();
}
