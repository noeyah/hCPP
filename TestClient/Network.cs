using Google.Protobuf;
using hCSharpLibrary.Network;
using System.Buffers;
using System.Diagnostics;
using System.Runtime.InteropServices;
using Packet;

namespace TestClient;

public class Network
{
	public SessionCallback? NetConnectedCallback;
	public ReceiveCallback? NetReceivedCallback;
	public Action? DisconnectCallback;

	private NetClient? _netClient;
	private MemoryPool<byte> _memoryPool = MemoryPool<byte>.Shared;

	private ISession? _server;

	public void Init(NetClientConfig config)
	{
		_netClient = new NetClient(config);

		_netClient.SessionConnected += OnConnected;
		_netClient.SessionDisconnected += OnDisconnected;
		_netClient.SessionReceived += OnReceived;
	}

	public void Start()
	{
		_netClient?.Start();
	}

	public void Send<TPacket>(TPacket packet) where TPacket : IMessage<TPacket>
	{
		if (_server is null)
		{
			return;
		}

		PacketID packetId = PacketHelper.GetID(packet);

		var bodySize = packet.CalculateSize();
		var bufferSize = NetworkDefine.HEADER_SIZE + bodySize;

		var owner = _memoryPool.Rent(bufferSize);

		try
		{
			int serializeSize = PacketHelper.Serialize(packetId, packet, owner.Memory.Slice(0, bufferSize).Span);
			Debug.Assert(bufferSize == serializeSize);

			_server.Send(owner, bufferSize);
		}
		catch (Exception ex)
		{
			Console.WriteLine(ex.Message);
			owner?.Dispose();
			throw;
		}
	}

	private void OnConnected(ISession session)
	{
		_server = session;

		NetConnectedCallback?.Invoke(session);
	}

	private void OnDisconnected(ISession session)
	{
		DisconnectCallback?.Invoke();
		_server = null;
	}

	private void OnReceived(ISession session, ushort packetId, ReadOnlySpan<byte> body)
	{
		NetReceivedCallback?.Invoke(session, packetId, body);
	}

}
