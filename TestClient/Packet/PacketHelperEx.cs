using Google.Protobuf;
using hCSharpLibrary.Network;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace Packet;

public static partial class PacketHelper
{

	private static PacketID GetID(Type type)
	{
		if (_typeToId.TryGetValue(type, out PacketID packetID))
		{
			return packetID;
		}
		return PacketID.None;
	}

	public static PacketID GetID<T>(T packet) where T : IMessage<T>
	{
		return GetID(typeof(T));
	}

	public static PacketID GetID<T>() where T : IMessage<T>
	{
		return GetID(typeof(T));
	}

	public static T? Deserialize<T>(ushort packetId, ReadOnlySpan<byte> body) where T : class, IMessage<T>
	{
		if ( _idToParser.TryGetValue(packetId, out var parser))
		{
			try
			{
				var message = parser.ParseFrom(body);
				return message as T;
			}
			catch (Exception ex)
			{
				Console.WriteLine(ex.Message);
				throw;
			}
		}

		return null;
	}

	public static int Serialize<T>(PacketID packetId, T packet, Span<byte> targetBuffer) where T : IMessage<T>
	{
		var packetSize = packet.CalculateSize();
		var bufferSize = NetworkDefine.HEADER_SIZE + packetSize;
		Debug.Assert(targetBuffer.Length >= bufferSize);

		PacketHeader header = new()
		{
			Id = (ushort)packetId,
			Size = (ushort)bufferSize
		};

		MemoryMarshal.Write(targetBuffer.Slice(0, NetworkDefine.HEADER_SIZE), header);
		packet.WriteTo(targetBuffer.Slice(NetworkDefine.HEADER_SIZE, packetSize));

		return bufferSize;
	}
}
