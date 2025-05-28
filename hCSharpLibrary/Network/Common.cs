using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace hCSharpLibrary.Network;

public class NetworkDefine
{
	public const int MAX_BUFFER_SIZE = 4096;
	public static readonly int HEADER_SIZE = Unsafe.SizeOf<PacketHeader>();
}

[StructLayout(LayoutKind.Sequential, Pack = 1, Size = 4)] // ?????
public struct PacketHeader
{
	public ushort Size;
	public ushort Id;
}

public class NetConfig
{
	public string Name = string.Empty;
	public string IP = string.Empty;
	public int Port;
}

public class NetServerConfig : NetConfig
{
	public int Backlog;
	public int AcceptCount;
	public int SAEAPoolingCount;
}

public class NetClientConfig : NetConfig
{
	public int ConnectCount = 1;
}