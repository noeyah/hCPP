using hCSharpLibrary.Network;
using Packet;
using System.Diagnostics;

namespace TestClient;

public partial class WinLobby : Form
{
	public event Action<List<(ulong, string)>>? OpenWinChat;
	public event Action<List<(ulong, string)>>? OpenWinText;

	private Network _network;

	public WinLobby(Network network)
	{
		_network = network;
		_network.NetReceivedCallback += OnReceived;
		InitializeComponent();
	}

	private void button_join_Click(object sender, EventArgs e)
	{
		var name = textBox_name.Text;
		if (string.IsNullOrWhiteSpace(name))
		{
			MessageBox.Show("이름을 확인해주세요.", "에러");
			return;
		}

		if (name.Length > (int)Defaults.NameMaxLimit)
		{
			MessageBox.Show($"이름은 {(int)Defaults.NameMaxLimit}자까지 가능합니다.", "에러");
		}

		JoinRoomReq req = new JoinRoomReq();
		req.Name = name;

		_network.Send(req);
	}

	private void OnReceived(ISession session, ushort packetId, ReadOnlySpan<byte> body)
	{
		switch ((PacketID)packetId)
		{
			case PacketID.JoinRoomRes:
				{
					JoinRoomRes? res = PacketHelper.Deserialize<JoinRoomRes>(packetId, body);
					Debug.Assert(res != null);
					OnReceived(session, res);
				}
				break;
		}
	}

	private void OnReceived(ISession session, JoinRoomRes res)
	{
		if (res.ErrorCode != ErrorCode.Ok)
		{
			this.ShowMsgBox($"{res.ErrorCode}", "서버 에러");
			return;
		}

		var list = res.Players.Select(v => (v.SessionId, v.Name)).ToList();

		if (radioButton_chat.Checked)
		{
			this.DrawUI(() =>
			{
				OpenWinChat?.Invoke(list);
				this.Close();
			});
		}
		else
		{
			this.DrawUI(() =>
			{
				OpenWinText?.Invoke(list);
				this.Close();
			});
		}
		
	}

}
