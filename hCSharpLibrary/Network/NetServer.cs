
namespace hCSharpLibrary.Network;

public class NetServer : NetService
{
	private Listener _listener = new();
	private NetServerConfig _config;


	public NetServer(NetServerConfig config) : base(config.SAEAPoolingCount)
	{
		_listener.OnAcceptCallback += OnConnectNewSession;
		_config = config;
	}

	public override void Start()
	{
		_listener.Start(_config.IP, _config.Port, _config.Backlog, _config.AcceptCount);
	}

	public override void Stop()
	{
		base.Stop();
		_listener.Stop();
	}
}
