using Google.Protobuf;
using hCSharpLibrary.Network;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Options;
using Packet;

namespace TestServer;

internal class MainServer : IHostedService
{
	private readonly NetServer _server;
	private readonly ServerSettings _settings;

	private readonly PacketProcessor _packetProcessor;

	public MainServer(IOptions<ServerSettings> settings, PacketProcessor packetProcessor)
	{
		_settings = settings.Value;
		_packetProcessor = packetProcessor;

		var netConfig = new NetServerConfig
		{
			Name = "Test Server",
			IP = _settings.IP,
			Port = _settings.Port,
			Backlog = _settings.Backlog,
			AcceptCount = _settings.AcceptCount,
			SAEAPoolingCount = _settings.SAEAPoolingCount
		};

		_server = new NetServer(netConfig);
		_server.SessionConnected += OnConnected;
		_server.SessionDisconnected += OnDisconnected;
		_server.SessionReceived += OnReceived;
	}

	public Task StartAsync(CancellationToken cancellationToken)
	{
		_server.Start();
		_packetProcessor.Start(_settings.TaskCount);

		Console.WriteLine("Test Server Start");
		return Task.CompletedTask;
	}

	public Task StopAsync(CancellationToken cancellationToken)
	{
		_server.Stop();
		return Task.CompletedTask;
	}

	private void OnConnected(ISession session)
	{
	}

	private void OnDisconnected(ISession session)
	{
		_packetProcessor.RecvPacket(new PacketData
		{
			Session = session,
			PacketID = 0,
			Message = null
		});
	}

	private void OnReceived(ISession session, ushort packetId, ReadOnlySpan<byte> body)
	{
		IMessage? msg = PacketHelper.Deserialize(packetId, body);
		if (msg == null)
		{
			Console.WriteLine($"Failed Deserialize PacketID {(PacketID)packetId}");
			return;
		}

		_packetProcessor.RecvPacket(new PacketData
		{
			Session = session,
			PacketID = packetId,
			Message = msg
		});
	}
}
