using System;
using System.Buffers;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Net.Sockets;
using System.Runtime.InteropServices;

namespace hCSharpLibrary.Network;

internal class Session : ISession, IDisposable
{
	internal event SessionCallback? OnCloseCallback;
	internal event ReceiveCallback? OnReceivedCallback;

	private readonly long _sessionId;
	private Socket _socket;

	private readonly DateTime _startTime;

	private readonly MemoryPool<byte> _memoryPool = MemoryPool<byte>.Shared;
	private readonly SocketAsyncEventArgsPool _saeaPool;

	// recv
	private SocketAsyncEventArgs _recvArgs;
	private BufferReader _bufferReader;

	// send
	private SocketAsyncEventArgs _sendArgs;
	private ConcurrentQueue<SendBuffer> _sendQueue = new();
	private int _isSending = 0;

	private int _state = 0;

	protected int _isDisposed = 0;

	const int MAX_SEND_BUFFER_COUNT = 5;

	public long SessionID => _sessionId;

	public Session(long sessionId, Socket socket, SocketAsyncEventArgsPool saeaPool)
	{
		_sessionId = sessionId;
		_socket = socket;
		_startTime = DateTime.Now;
		_bufferReader = new(NetworkDefine.MAX_BUFFER_SIZE);
		_saeaPool = saeaPool;

		_recvArgs = _saeaPool.Get();
		_recvArgs.Completed += new EventHandler<SocketAsyncEventArgs>(RecvCompleted);

		_sendArgs = _saeaPool.Get();
		_sendArgs.Completed += new EventHandler<SocketAsyncEventArgs>(SendCompleted);

		_state = 1;
	}

	public void Start()
	{
		var owner = _memoryPool.Rent(NetworkDefine.MAX_BUFFER_SIZE);
		_recvArgs.UserToken = owner;
		PostRecv(_recvArgs);
	}

	#region Send

	public void Send(Span<byte> data)
	{
		if (data.IsEmpty)
		{
			return;
		}

		var sendBuffer = new SendBuffer(data, _memoryPool);
		Send(sendBuffer);
	}

	public void Send(IMemoryOwner<byte> memoryOwner, int bufferSize)
	{
		if (bufferSize == 0)
		{
			memoryOwner.Dispose();
			return;
		}

		var sendBuffer = new SendBuffer(memoryOwner, bufferSize);
		Send(sendBuffer);
	}

	private void Send(SendBuffer sendBuffer)
	{
		if (!IsConnected())
		{
			sendBuffer.Dispose();
			return;
		}
		
		_sendQueue.Enqueue(sendBuffer);

		if (CanSend())
		{
			if (!PostSend())
			{
				SendEnd();
			}
		}
	}

	private bool PostSend()
	{
		if (!IsConnected())
		{
			return false;
		}

		if (!SetSendArgs())
		{
			return false;
		}

		if (!_socket.SendAsync(_sendArgs))
		{
			SendCompleted(null, _sendArgs);
		}

		return true;
	}

	private bool SetSendArgs()
	{
		_sendArgs.BufferList = null;
		_sendArgs.SetBuffer(null, 0, 0);
		_sendArgs.UserToken = null;

		List<SendBuffer> sendBufferList = new();

		if (_sendQueue.Count == 1)
		{
			if (!_sendQueue.TryDequeue(out var buffer))
			{
				return false;
			}
			_sendArgs.SetBuffer(buffer.Buffer);
			sendBufferList.Add(buffer);
			_sendArgs.UserToken = sendBufferList;
			return true;
		}

		List<ArraySegment<byte>> bufferList = new();
		int totalBytes = 0;

		while (bufferList.Count < MAX_SEND_BUFFER_COUNT
			&& totalBytes < NetworkDefine.MAX_BUFFER_SIZE
			&& _sendQueue.TryDequeue(out var sendBuffer))
		{
			if (!MemoryMarshal.TryGetArray(sendBuffer.Buffer, out ArraySegment<byte> segment))
			{
				if (Debugger.IsAttached)
				{
					Debugger.Break();
				}
				throw new ApplicationException("Error : Failed Memory<byte> To ArraySegment<byte> !");
			}

			bufferList.Add(segment);
			sendBufferList.Add(sendBuffer);
			totalBytes += segment.Count;
		}

		if (bufferList.Count == 0)
		{
			return false;
		}

		_sendArgs.BufferList = bufferList;
		_sendArgs.UserToken = sendBufferList;
		return true;
	}

	private void SendCompleted(object? sender, SocketAsyncEventArgs args)
	{
		ClearSendArgs();

		if (args.BytesTransferred == 0 || args.SocketError != SocketError.Success)
		{
			Console.WriteLine(args.SocketError.ToString());
			SendEnd();
			Close();
			return;
		}

		if (!_sendQueue.IsEmpty)
		{
			if (!PostSend())
			{
				SendEnd();
			}
		}
		else
		{
			SendEnd();
		}
	}

	#endregion // Send

	#region Recv
	private void PostRecv(SocketAsyncEventArgs args)
	{
		if (!IsConnected())
		{
			return;
		}

		if (args.UserToken is IMemoryOwner<byte> owner)
		{
			args.SetBuffer(owner.Memory);
		}
		else
		{
			throw new InvalidOperationException("recv user token is not IMemoryOwner");
		}

		bool pending = _socket.ReceiveAsync(args);
		if (!pending)
		{
			RecvCompleted(null, args);
		}
	}

	private void RecvCompleted(object? sender, SocketAsyncEventArgs args)
	{
		if (args.SocketError != SocketError.Success || args.BytesTransferred == 0)
		{
			Close();
			return;
		}

		_bufferReader.Write(args.MemoryBuffer.Span.Slice(args.Offset, args.BytesTransferred));
		ReadBuffer();
		_bufferReader.Cleanup();

		PostRecv(args);
	}

	private void ReadBuffer()
	{
		while (true)
		{
			if ( _bufferReader.DataSize < NetworkDefine.HEADER_SIZE)
			{
				break;
			}

			var buffer = _bufferReader.DataSpan;
			PacketHeader header = MemoryMarshal.Read<PacketHeader>(buffer);
			Debug.Assert(header.Size <= NetworkDefine.MAX_BUFFER_SIZE, $"packet size {header.Size} > {NetworkDefine.MAX_BUFFER_SIZE}");

			if ( header.Size > _bufferReader.DataSize)
			{
				break;
			}

			var body = buffer[NetworkDefine.HEADER_SIZE..header.Size];
			OnReceivedCallback?.Invoke(this, header.Id, body);
			_bufferReader.CommitRead(header.Size);
		}
	}

	#endregion // Recv

	#region state
	private bool IsConnected()
	{
		return Volatile.Read(ref _state) == 1;
	}

	private bool CanClose()
	{
		if (Interlocked.CompareExchange(ref _state, 0, 1) == 1)
		{
			return true;
		}
		return false;
	}

	private bool CanSend()
	{
		if (Interlocked.CompareExchange(ref _isSending, 1, 0) == 0)
		{
			return true;
		}
		return false;
	}

	private void SendEnd()
	{
		Interlocked.Exchange(ref _isSending, 0);
	}

	#endregion

	#region Clear
	
	private void ClearRecvArgs()
	{
		if (_recvArgs.UserToken == null)
		{
			return;
		}

		var owner = _recvArgs.UserToken as IMemoryOwner<byte>;
		owner?.Dispose();
		_recvArgs.UserToken = null;
	}

	private void ClearSendArgs()
	{
		if (_sendArgs.UserToken is List<SendBuffer> sendBuffers)
		{
			foreach (var sendBuffer in sendBuffers)
			{
				sendBuffer.Dispose();
			}
		}

		_sendArgs.UserToken = null;
		_sendArgs.SetBuffer(null, 0, 0);
		_sendArgs.BufferList = null;
	}

	public void Close()
	{
		if (!CanClose())
		{
			return;
		}

		OnCloseCallback?.Invoke(this);

		try
		{
			_socket.Shutdown(SocketShutdown.Both);
		}
		catch
		{
		}

		Dispose();
	}

	public void Dispose()
	{
		Dispose(true);
		GC.SuppressFinalize(this);
	}

	protected virtual void Dispose(bool disposing)
	{
		if (Interlocked.CompareExchange(ref _isDisposed, 1, 0) == 1)
		{
			return;
		}

		if (disposing)
		{
			OnCloseCallback = null;
			OnReceivedCallback = null;

			_socket?.Close();
			_socket = null!;

			while (_sendQueue.TryDequeue(out var buffer))
			{
				buffer.Dispose();
			}
			_sendQueue.Clear();
			_sendQueue = null!;

			ClearRecvArgs();
			_saeaPool.Return(_recvArgs);

			ClearSendArgs();
			_saeaPool.Return(_sendArgs);
			
			_bufferReader = null!;
		}
	}

	#endregion
}
