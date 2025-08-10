
namespace TestServer;

public class ServerSettings
{
	public string IP { get; set; }
	public int Port { get; set; }
	public int Backlog { get; set; }
	public int AcceptCount { get; set; }
	public int SAEAPoolingCount { get; set; }
	public int TaskCount { get; set; }
}