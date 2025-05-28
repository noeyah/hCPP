using System.Buffers;

namespace hCSharpLibrary.Network;

public interface ISession
{
	long SessionID { get; }

	void Send(IMemoryOwner<byte> memoryOwner, int bufferSize);

	void Send(Span<byte> data);
}
