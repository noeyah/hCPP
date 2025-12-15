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
	config.ioThreadCount = 2;

	MainServer::Instance()->Init(config);
	MainServer::Instance()->Start();

	core::CmdService cmdService;
	cmdService.RegisterCommand("exit", "exit server",
							   [&cmdService](const core::CommandArgs& args) {
									MainServer::Instance()->Stop();
									cmdService.Exit();
							   });

	cmdService.Run();
}
