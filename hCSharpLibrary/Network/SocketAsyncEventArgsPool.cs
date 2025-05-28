using System.Collections.Concurrent;
using System.Net.Sockets;

namespace hCSharpLibrary.Network;

internal class SocketAsyncEventArgsPool
{
	private readonly ConcurrentQueue<SocketAsyncEventArgs> _pool = new();

	public SocketAsyncEventArgsPool(int count)
	{
		for (int i = 0; i < count; i++)
		{
			_pool.Enqueue(new SocketAsyncEventArgs());
		}
	}

	public SocketAsyncEventArgs Get()
	{
		if (_pool.TryDequeue(out var args))
		{
			return args;
		}

		return new SocketAsyncEventArgs();
	}

	public void Return(SocketAsyncEventArgs args)
	{
		if (args == null)
		{
			return;
		}

		args.AcceptSocket = null;
		args.BufferList = null;
		args.SetBuffer(null, 0, 0);
		args.UserToken = null;
		args.RemoteEndPoint = null;

		_pool.Enqueue(args);
	}
}
