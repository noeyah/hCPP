#include "core.h"
#include <atomic>
#include <csignal>
#include "Server.h"
#include "Packet/PacketRegister.h"
#include "Server/DummyManager.h"

std::atomic_bool g_bRunning{ true };

void SignalHandler(int signal)
{
	LOG_INFO("Signal {} received, shut down...", signal);
	g_bRunning = false;
}

int main()
{
	signal(SIGINT, SignalHandler);
	signal(SIGTERM, SignalHandler);

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	hlib::Memory::Instance();
	PacketRegister::Initialize();

	hlib::NetConfig config;
	config.name = "dummy client";
	config.ip = "127.0.0.1";
	config.port = 7777;

	Server::Instance()->Init(config);
	Server::Instance()->Start();

	DummyManager dummyMgr;
	dummyMgr.CreateDummies(100);

	while (g_bRunning)
	{
		dummyMgr.Update();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	LOG_INFO("shut down......");
	dummyMgr.Clear();
	Server::Instance()->Stop();
	return 0;
}
