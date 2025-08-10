using Google.Protobuf;
using hCSharpLibrary.Network;
using Packet;

namespace TestServer;

public interface IPacketHandler
{
	void Handle(ISession session, IMessage message);
	//Task HandleAsync(ISession session, IMessage message);
}

internal class PacketHandler
{
	private readonly Dictionary<ushort, IPacketHandler> _handlers = new();

	public PacketHandler(RoomManager roomManager)
	{
		// 패킷 핸들러 등록
		_handlers.Add((ushort)PacketID.JoinRoomReq, new JoinRoomHandler(roomManager));
		_handlers.Add((ushort)PacketID.LeaveRoomReq, new LeaveRoomHandler(roomManager));
		_handlers.Add((ushort)PacketID.ChatReq, new ChatHandler(roomManager));

		// disconnect 추가할까...
		_handlers.Add((ushort)PacketID.None, new DisconnectHandler(roomManager));
	}

	public void Handle(PacketData packetData)
	{
		if (_handlers.TryGetValue(packetData.PacketID, out var handler))
		{
			handler.Handle(packetData.Session, packetData.Message);
		}
	}

}
