using hCSharpLibrary.Network;

namespace TestClient;

public partial class WinConnect : Form
{
	public event Action? OpenWinLobby;
	
	private Network _network;

	public WinConnect(Network network)
	{
		_network = network;
		_network.NetConnectedCallback += OnConnected;

		InitializeComponent();
	}

	private void button_connect_Click(object sender, EventArgs e)
	{
		var ip = textBox_ip.Text;
		var strPort = textBox_port.Text;

		if ( string.IsNullOrWhiteSpace( ip ) 
			|| string.IsNullOrWhiteSpace( strPort ))
		{
			MessageBox.Show("IP, Port를 확인해주세요.", "에러");
			return;
		}

		if (!int.TryParse( strPort, out int port ))
		{
			MessageBox.Show("Port가 잘못되었습니다.", "에러");
			return;
		}

		NetClientConfig config = new()
		{
			IP = ip,
			Port = port,
			Name = "TestClient"
		};

		_network.Init(config);
		_network.Start();
	}

	private void OnConnected(ISession session)
	{
		this.DrawUI(() =>
		{
			OpenWinLobby?.Invoke();
			this.Close();
		});
	}

}
