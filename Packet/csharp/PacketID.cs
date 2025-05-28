﻿namespace Packet;

public enum PacketID : ushort
{
	None = 0,
	ChatNot = 1,
	ChatReq = 2,
	ChatRes = 3,
	JoinRoomNot = 4,
	JoinRoomReq = 5,
	JoinRoomRes = 6,
	LeaveRoomNot = 7,
	LeaveRoomReq = 8,
	LeaveRoomRes = 9,
}