
namespace hCSharpLibrary.Network;

public class NetClient : NetService
{
	private Connector _connector = new();
	private NetClientConfig _config;

	public NetClient(NetClientConfig config) : base(config.ConnectCount * 2)
	{
		_connector.OnServerConnectCallback += OnConnectNewSession;
		_config = config;
	}

	public override void Start()
	{
		for (int i = 0; i < _config.ConnectCount; i++)
		{
			_connector.Connect(_config.IP, _config.Port);
		}
	}

	public override void Stop()
	{
		base.Stop();
	}
}
