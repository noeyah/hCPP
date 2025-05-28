using hCSharpLibrary.Network;

namespace TestClient;

internal class AppContext : ApplicationContext
{
	private Network _network;

	public AppContext()
	{
		_network = new Network();
		_network.DisconnectCallback = OnDisconnected;
		ShowWinConnect();
	}

	private void ShowWinConnect()
	{
		var winConnect = new WinConnect(_network);
		winConnect.OpenWinLobby += ShowWinLobby;
		this.MainForm = winConnect;
		winConnect.Show();
	}

	private void ShowWinLobby()
	{
		var winLobby = new WinLobby(_network);
		winLobby.OpenWinChat += ShowWinChat;
		winLobby.OpenWinText += ShowWinText;
		winLobby.StartPosition = FormStartPosition.Manual;
		winLobby.Location = this.MainForm!.Location;
		this.MainForm = winLobby;
		winLobby.Show();
	}

	private void ShowWinChat(List<(ulong, string)> players)
	{
		var winChat = new WinChat(_network);
		winChat.OpenWinLobby += ShowWinLobby;
		winChat.SetPlayers(players);
		winChat.StartPosition = FormStartPosition.Manual;
		winChat.Location = this.MainForm!.Location;
		this.MainForm = winChat;
		winChat.Show();
	}

	private void ShowWinText(List<(ulong, string)> players)
	{
		var winText = new WinText(_network);
		winText.OpenWinLobby += ShowWinLobby;
		winText.SetPlayers(players);
		winText.StartPosition = FormStartPosition.Manual;
		winText.Location = this.MainForm!.Location;
		this.MainForm = winText;
		winText.Show();
	}

	private void OnDisconnected()
	{
		this.MainForm?.DrawUI(() =>
		{
			MessageBox.Show("서버랑 연결이 끊겼습니다.", "Disconnect");
			this.MainForm?.Close();
		});
	}
}
