using System;
using System.Buffers;
using System.Net.Sockets;

namespace hCSharpLibrary.Network;

public delegate void SessionCallback(ISession session);
public delegate void ReceiveCallback(ISession session, ushort packetId, ReadOnlySpan<byte> body);
internal delegate void NewSessionCallback(Socket socket);

public abstract class NetService
{
	public event SessionCallback? SessionConnected;
	public event SessionCallback? SessionDisconnected;
	public event ReceiveCallback? SessionReceived;

	private SocketAsyncEventArgsPool _saeaPool;
	private MemoryPool<byte> _memoryPool = MemoryPool<byte>.Shared;
	private SessionManager _sessionManager = new();

	public NetService(int saeaPoolingCount)
	{
		_saeaPool = new SocketAsyncEventArgsPool(saeaPoolingCount);
	}

	public abstract void Start();
	public virtual void Stop()
	{
		_sessionManager.CloseAll();
	}

	public ISession? GetSession(long sessionId)
	{
		return _sessionManager.GetSession(sessionId);
	}

	internal void OnConnectNewSession(Socket socket)
	{
		var sessionId = _sessionManager.GenerateSessionId();
		var newSession = new Session(sessionId, socket, _memoryPool, _saeaPool);
		
		newSession.OnCloseCallback += OnCloseSession;
		newSession.OnReceivedCallback += SessionReceived;

		_sessionManager.Add(newSession);

		Task.Run(() => SessionConnected?.Invoke(newSession));

		newSession.Start();
	}

	private void OnCloseSession(ISession session)
	{
		SessionDisconnected?.Invoke(session);
		_sessionManager.Remove(session.SessionID);
	}

}
