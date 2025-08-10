using Google.Protobuf;
using hCSharpLibrary.Network;
using Packet;

namespace TestServer;

internal class DisconnectHandler : IPacketHandler
{
	private readonly RoomManager _roomManager;

	public DisconnectHandler(RoomManager roomManager)
	{
		_roomManager = roomManager;
	}

	public void Handle(ISession session, IMessage message)
	{
		Console.WriteLine($"Session({session.SessionID}) : disconnected");

		var room = _roomManager.GetRoom();

		if (!room.IsMember(session.SessionID))
		{
			return;
		}

		room.LeaveUser(session.SessionID);

		LeaveRoomNot leaveNot = new LeaveRoomNot();
		leaveNot.SessionId = (ulong)session.SessionID;
		room.Broadcast(leaveNot, session.SessionID);
	}
}