#include "Helper.h"
#include "MainServer.h"
#include "Room.h"

namespace Helper
{
	std::shared_ptr<Room> GetRoom()
	{
		return MainServer::Instance()->GetRoom();
	}
}