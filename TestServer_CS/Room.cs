using Google.Protobuf;
using hCSharpLibrary.Network;
using System.Collections.Concurrent;

namespace TestServer;

internal class Room
{
	private ConcurrentDictionary<long, User> _dicUser = new();

	public bool IsMember(long uid)
	{
		return _dicUser.ContainsKey(uid);
	}

	public void JoinUser(long uid, string name, ISession session)
	{
		_dicUser.TryAdd(uid, new User(uid, name, session));
	}

	public void LeaveUser(long uid)
	{
		_dicUser.TryRemove(uid, out _);
	}

	public IEnumerable<User> GetUsers()
	{
		return _dicUser.Values;
	}

	public void Broadcast<T>(T message, long excludeUid) where T : IMessage<T>
	{
		var users = _dicUser.Values.ToList();
		foreach (var user in users)
		{
			if (user.UID == excludeUid)
				continue;

			user.Send(message);
		}
	}
}
