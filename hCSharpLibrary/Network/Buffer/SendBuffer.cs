using System.Buffers;

namespace hCSharpLibrary.Network;

internal class SendBuffer : IDisposable
{
	internal Memory<byte> Buffer { get; private set; }
	internal IMemoryOwner<byte>? Owner { get; private set; }

	internal SendBuffer(IMemoryOwner<byte> owner, int length)
	{
		Owner = owner;
		Buffer = owner.Memory.Slice(0, length);
	}

	internal SendBuffer(Span<byte> span, MemoryPool<byte> pool)
	{
		Owner = pool.Rent(span.Length);
		span.CopyTo(Owner.Memory.Span);
		Buffer = Owner.Memory.Slice(0, span.Length);
	}

	public void Dispose()
	{
		Owner?.Dispose();
		Owner = null;
	}
}
