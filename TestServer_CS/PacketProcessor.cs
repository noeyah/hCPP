using Google.Protobuf;
using hCSharpLibrary.Network;
using System.Threading.Channels;

namespace TestServer;

public struct PacketData
{
	public ushort PacketID;
	public ISession Session;
	public IMessage Message;
}

internal class PacketProcessor
{
	private readonly Channel<PacketData> _channel = Channel.CreateUnbounded<PacketData>();
	private readonly PacketHandler _packetHandler;

	public PacketProcessor(PacketHandler packetHandler)
	{
		_packetHandler = packetHandler;
	}

	public void RecvPacket(PacketData packetData)
	{
		_channel.Writer.TryWrite(packetData);
	}

	public void Start(int workerCount)
	{
		for (int i = 0; i < workerCount; i++)
		{
			Task.Run(ProcessAsync);
		}
	}

	private async Task ProcessAsync()
	{
		await foreach (var packet in _channel.Reader.ReadAllAsync())
		{
			try
			{
				_packetHandler.Handle(packet);
			}
			catch (Exception ex)
			{
				Console.WriteLine($"Packet processing error : {ex.Message}");
			}
		}
	}

}
