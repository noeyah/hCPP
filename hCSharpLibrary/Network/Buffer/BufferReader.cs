namespace hCSharpLibrary.Network;

internal class BufferReader
{
	const int BUFFER_SPARE = 3;

	private byte[] _buffer;
	private int _readIdx = 0;
	private int _writeIdx = 0;

	public int DataSize => _writeIdx - _readIdx;
	public int RemainSize => _buffer.Length - _writeIdx;

	public Span<byte> DataSpan => _buffer.AsSpan()[_readIdx.._writeIdx];
	public Memory<byte> DataMemory => _buffer.AsMemory()[_readIdx.._writeIdx];
	public Span<byte> FreeSpan => _buffer.AsSpan()[_writeIdx.._buffer.Length];

	public BufferReader(int bufferSize)
	{
		_buffer = new byte[bufferSize * BUFFER_SPARE];
	}

	public bool Write(Span<byte> buffer)
	{
		if (buffer.Length > RemainSize)
		{
			return false;
		}

		buffer.CopyTo(FreeSpan);
		_writeIdx += buffer.Length;
		return true;
	}

	public bool CommitRead(int size)
	{
		if (size > DataSize)
		{
			return false;
		}

		_readIdx += size;
		return true;
	}

	public void Cleanup()
	{
		if (DataSize == 0)
		{
			_readIdx = 0;
			_writeIdx = 0;
			return;
		}

		DataSpan.CopyTo(_buffer.AsSpan());

		_writeIdx = DataSize;
		_readIdx = 0;
	}
}
