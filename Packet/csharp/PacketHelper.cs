// 이 파일은 스크립트로 생성됨
// 위치 : Solution/Packet/jinja2/csharp

using Google.Protobuf;

namespace Packet;

public static partial class PacketHelper
{
	private static readonly Dictionary<Type, PacketID> _typeToId = new();
	private static readonly Dictionary<ushort, MessageParser> _idToParser = new();

	static PacketHelper()
	{
		// Type To PacketID 초기화 코드
		_typeToId.Add(typeof(ChatNot), PacketID.ChatNot);
		_typeToId.Add(typeof(ChatReq), PacketID.ChatReq);
		_typeToId.Add(typeof(ChatRes), PacketID.ChatRes);
		_typeToId.Add(typeof(JoinRoomNot), PacketID.JoinRoomNot);
		_typeToId.Add(typeof(JoinRoomReq), PacketID.JoinRoomReq);
		_typeToId.Add(typeof(JoinRoomRes), PacketID.JoinRoomRes);
		_typeToId.Add(typeof(LeaveRoomNot), PacketID.LeaveRoomNot);
		_typeToId.Add(typeof(LeaveRoomReq), PacketID.LeaveRoomReq);
		_typeToId.Add(typeof(LeaveRoomRes), PacketID.LeaveRoomRes);

		// PacketID To Parser 초기화 코드
		_idToParser.Add((ushort)PacketID.ChatNot, ChatNot.Parser);
		_idToParser.Add((ushort)PacketID.ChatReq, ChatReq.Parser);
		_idToParser.Add((ushort)PacketID.ChatRes, ChatRes.Parser);
		_idToParser.Add((ushort)PacketID.JoinRoomNot, JoinRoomNot.Parser);
		_idToParser.Add((ushort)PacketID.JoinRoomReq, JoinRoomReq.Parser);
		_idToParser.Add((ushort)PacketID.JoinRoomRes, JoinRoomRes.Parser);
		_idToParser.Add((ushort)PacketID.LeaveRoomNot, LeaveRoomNot.Parser);
		_idToParser.Add((ushort)PacketID.LeaveRoomReq, LeaveRoomReq.Parser);
		_idToParser.Add((ushort)PacketID.LeaveRoomRes, LeaveRoomRes.Parser);
	}
}