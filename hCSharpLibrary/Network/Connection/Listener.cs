using System;
using System.Net;
using System.Net.Sockets;

namespace hCSharpLibrary.Network;

internal class Listener
{
	public event NewSessionCallback? OnAcceptCallback;

	private Socket? _socket;
	private int _isRunning = 0;

	private bool IsRunning => _isRunning == 1;

	public void Start(string ip, int port, int backlog, int acceptCount)
	{
		if (Interlocked.Exchange(ref _isRunning, 1) == 1)
		{
			return;
		}

		try
		{
			IPEndPoint endPoint = new IPEndPoint(IPAddress.Parse(ip), port);
			_socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

			_socket.Bind(endPoint);
			_socket.Listen(backlog);

			for (int i = 0; i < acceptCount; i++)
			{
				var args = new SocketAsyncEventArgs();
				args.Completed += new EventHandler<SocketAsyncEventArgs>(CompletedAccept);

				PostAccept(args);
			}
		}
		catch (Exception e)
		{
			Console.WriteLine(e);
		}
	}

	public void Stop()
	{
		if ( Interlocked.Exchange(ref _isRunning, 0) == 0)
		{
			return;
		}
			
		_socket?.Close();
		_socket = null;
	}

	private void PostAccept(SocketAsyncEventArgs args)
	{
		if (!IsRunning)
		{
			return;
		}

		args.AcceptSocket = null;

		if (!_socket!.AcceptAsync(args))
		{
			CompletedAccept(null, args);
		}
	}

	private void CompletedAccept(object? sender, SocketAsyncEventArgs args)
	{
		if (!IsRunning)
		{
			return;
		}

		if (args.SocketError == SocketError.Success)
		{
			var socket = args.AcceptSocket;
			if (socket is not null)
			{
				OnAcceptCallback?.Invoke(socket);
			}

			PostAccept(args);
		}
		else
		{
			Console.WriteLine($"Accept faile {args.SocketError.ToString()}");
			args.Dispose();
		}
	}
}
