using Google.Protobuf;
using hCSharpLibrary.Network;
using Packet;

namespace TestServer;

internal class JoinRoomHandler : IPacketHandler
{
	private readonly RoomManager _roomManager;

	public JoinRoomHandler(RoomManager roomManager)
	{
		_roomManager = roomManager;
	}

	public void Handle(ISession session, IMessage message)
	{
		if (message is not JoinRoomReq req)
			return;

		JoinRoomRes res = new JoinRoomRes();

		var room = _roomManager.GetRoom();

		if (room.IsMember(session.SessionID))
		{
			res.ErrorCode = ErrorCode.FailAlreadyInRoom;
			session.SendPacket(res);
			return;
		}

		if (string.IsNullOrWhiteSpace(req.Name))
		{
			res.ErrorCode = ErrorCode.FailEmptyName;
			session.SendPacket(res);
			return;
		}

		if (req.Name.Length > (int)Defaults.NameMaxLimit)
		{
			res.ErrorCode = ErrorCode.FailExceedNameLimit;
			session.SendPacket(res);
			return;
		}

		room.JoinUser(session.SessionID, req.Name, session);

		var players = room.GetUsers().Select(v => new Player() { SessionId = (ulong)v.UID, Name = v.Name});
		res.Players.AddRange(players);
		res.ErrorCode = ErrorCode.Ok;
		session.SendPacket(res);

		Console.WriteLine($"Session({session.SessionID}) : Join Room");

		JoinRoomNot joinNot = new JoinRoomNot();
		joinNot.Player = new Player()
		{
			SessionId = (ulong)session.SessionID,
			Name = req.Name
		};
		room.Broadcast(joinNot, session.SessionID);
	}
}