using System.Net;
using System.Net.Sockets;

namespace hCSharpLibrary.Network;

internal class Connector
{
	public event NewSessionCallback? OnServerConnectCallback;

	public void Connect(string ip, int port)
	{
		try
		{
			IPEndPoint endPoint = new IPEndPoint(IPAddress.Parse(ip), port);
			Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

			SocketAsyncEventArgs args = new();
			args.Completed += new EventHandler<SocketAsyncEventArgs>(CompletedConnect);
			args.RemoteEndPoint = endPoint;

			if (!socket.ConnectAsync(args))
			{
				CompletedConnect(null, args);
			}
		}
		catch (Exception ex)
		{
			Console.WriteLine(ex.ToString());
		}
	}

	private void CompletedConnect(object? sender, SocketAsyncEventArgs args)
	{
		if (args.SocketError != SocketError.Success)
		{
			Console.WriteLine(args.SocketError.ToString());
			return;
		}

		if (args.ConnectSocket is null)
		{
			Console.WriteLine("ConnectSocket is null");
			return;
		}

		OnServerConnectCallback?.Invoke(args.ConnectSocket);
	}
}
