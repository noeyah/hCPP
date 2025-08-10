using Google.Protobuf;
using hCSharpLibrary.Network;
using Packet;

namespace TestServer;

internal class ChatHandler : IPacketHandler
{
	private readonly RoomManager _roomManager;

	public ChatHandler(RoomManager roomManager)
	{
		_roomManager = roomManager;
	}

	public void Handle(ISession session, IMessage message)
	{
		if (message is not ChatReq req)
			return;

		ChatRes res = new ChatRes();
		var room = _roomManager.GetRoom();

		if (!room.IsMember(session.SessionID))
		{
			res.ErrorCode = ErrorCode.FailNotInRoom;
			session.SendPacket(res);
			return;
		}

		if (string.IsNullOrWhiteSpace(req.Desc))
		{
			res.ErrorCode = ErrorCode.FailEmptyMessage;
			session.SendPacket(res);
			return;
		}

		if (req.Desc.Length > (int)Defaults.MessageMaxLimit)
		{
			res.ErrorCode = ErrorCode.FailExceedMessageLimit;
			session.SendPacket(res);
			return;
		}

		res.ErrorCode = ErrorCode.Ok;
		res.Desc = req.Desc;
		session.SendPacket(res);

		Console.WriteLine($"Session({session.SessionID}) : Chat ...");

		ChatNot chatNot = new ChatNot();
		chatNot.SessionId = (ulong)session.SessionID;
		chatNot.Desc = req.Desc;
		room.Broadcast(chatNot, session.SessionID);
	}
}
