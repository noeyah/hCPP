using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;

namespace TestServer;

internal class Program
{
	static void Main(string[] args)
	{
		var builder = Host.CreateDefaultBuilder(args);

		builder.ConfigureServices((context, services) =>
		{
			var config = context.Configuration;
			services.Configure<ServerSettings>(config.GetSection("ServerSettings"));

			services.AddSingleton<RoomManager>();
			services.AddSingleton<PacketHandler>();
			services.AddSingleton<PacketProcessor>();

			services.AddHostedService<MainServer>();
		});

		var host = builder.Build();
		host.Run();
	}
}
