using System.Buffers;
using System.Collections.Concurrent;

namespace hCSharpLibrary.Network;

internal class SessionManager
{
	private static long _lastSessionId = 0;

	private readonly ConcurrentDictionary<long, Session> _dicSession = new();

	public Session? GetSession(long sessionId)
	{
		if (_dicSession.TryGetValue(sessionId, out var session))
		{
			return session;
		}
		return null;
	}

	public long GenerateSessionId()
	{
		return Interlocked.Increment(ref _lastSessionId);
	}

	public void Add(Session session)
	{
		_dicSession.TryAdd(session.SessionID, session);
	}

	public void Remove(long sessionId)
	{
		_dicSession.TryRemove(sessionId, out _);
	}

	public void CloseAll()
	{
		var sessions = _dicSession.Values;

		if (!sessions.Any())
		{
			return;
		}

		foreach (var session in sessions)
		{
			session.Close();
		}
	}
}
