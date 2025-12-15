#include "Helper.h"
#include "MainServer.h"
#include "Room.h"

namespace Helper
{
	Room* GetRoom()
	{
		return MainServer::Instance()->GetRoom();
	}
}