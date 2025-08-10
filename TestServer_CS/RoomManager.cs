using System.Collections.Concurrent;

namespace TestServer;

internal class RoomManager
{
	private ConcurrentDictionary<int, Room> _dicRoom = new();

	public RoomManager()
	{
		_dicRoom.TryAdd(0, new Room());
	}

	public Room GetRoom(int roomNum = 0)
	{
		//if (_dicRoom.TryGetValue(roomNum, out var room))
		//{
		//	return room;
		//}

		//return null;
		return _dicRoom[0];
	}
}
