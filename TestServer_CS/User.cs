using Google.Protobuf;
using hCSharpLibrary.Network;

namespace TestServer;

internal class User
{
	private long _uid;
	private string _name;
	private ISession _session;

	public long UID => _uid;
	public string Name => _name;

	public User(long uid, string name, ISession session)
	{
		_uid = uid;
		_name = name;
		_session = session;
	}

	public void Send<T>(T message) where T : IMessage<T>
	{
		_session.SendPacket<T>(message);
	}

}
