using Google.Protobuf;
using hCSharpLibrary.Network;
using Packet;

namespace TestServer;

internal class LeaveRoomHandler : IPacketHandler
{
	private readonly RoomManager _roomManager;

	public LeaveRoomHandler(RoomManager roomManager)
	{
		_roomManager = roomManager;
	}

	public void Handle(ISession session, IMessage message)
	{
		if (message is not LeaveRoomReq req)
			return;

		LeaveRoomRes res = new LeaveRoomRes();
		var room = _roomManager.GetRoom();

		if (!room.IsMember(session.SessionID))
		{
			res.ErrorCode = ErrorCode.FailNotInRoom;
			session.SendPacket(res);
			return;
		}

		room.LeaveUser(session.SessionID);
		res.ErrorCode = ErrorCode.Ok;
		session.SendPacket(res);

		Console.WriteLine($"Session({session.SessionID}) : Leave Room");

		LeaveRoomNot leaveNot = new LeaveRoomNot();
		leaveNot.SessionId = (ulong)session.SessionID;
		room.Broadcast(leaveNot, session.SessionID);
	}
}
