using Google.Protobuf;
using hCSharpLibrary.Network;
using Packet;
using System.Buffers;
using System.Diagnostics;

namespace TestServer;

public static class SessionExtensions
{
	private static readonly MemoryPool<byte> _memoryPool = MemoryPool<byte>.Shared;

	public static void SendPacket<T>(this ISession session, T packet) where T : IMessage<T>
	{
		PacketID packetId = PacketHelper.GetID(packet);
		var bodySize = packet.CalculateSize();
		var bufferSize = NetworkDefine.HEADER_SIZE + bodySize;

		var owner = _memoryPool.Rent(bufferSize);

		try
		{
			int serializeSize = PacketHelper.Serialize(packetId, packet, owner.Memory.Slice(0, bufferSize).Span);
			Debug.Assert(bufferSize == serializeSize);

			session.Send(owner, bufferSize);
		}
		catch (Exception ex)
		{
			Console.WriteLine(ex.Message);
			owner?.Dispose();
			throw;
		}
	}
}
