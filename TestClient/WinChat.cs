using Google.Protobuf;
using hCSharpLibrary.Network;
using Packet;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Windows.Forms;
using System.Xml.Linq;

namespace TestClient;

public partial class WinChat : Form
{
	public event Action? OpenWinLobby;
	private Network _network;

	private ConcurrentDictionary<ulong, string> _players = new();

	private ConcurrentQueue<Control> _controls = new();
	private readonly System.Windows.Forms.Timer _timer;
	private const int MAX_BUBBLE_COUNT = 200;

	public WinChat(Network network)
	{
		_network = network;
		_network.NetReceivedCallback += OnReceived;
		InitializeComponent();
		flowLayoutPanel_main.HorizontalScroll.Enabled = false;
		flowLayoutPanel_main.HorizontalScroll.Visible = false;
		flowLayoutPanel_players.HorizontalScroll.Enabled = false;
		flowLayoutPanel_players.HorizontalScroll.Visible = false;

		AddSystemMsg("입장했습니다.");

		_timer = new System.Windows.Forms.Timer();
		_timer.Interval = 200;
		_timer.Tick += UpdateBubble;
		_timer.Start();
	}

	#region events
	private void button_send_Click(object sender, EventArgs e)
	{
		if (string.IsNullOrWhiteSpace(richTextBox_bottom.Text))
		{
			richTextBox_bottom.Reset();
			return;
		}

		if (richTextBox_bottom.Text.Length > (int)Defaults.MessageMaxLimit)
		{
			MessageBox.Show("문자 길이 초과", "에러");
			return;
		}

		ChatReq req = new ChatReq();
		req.Desc = richTextBox_bottom.Text;
		richTextBox_bottom.Reset();

		_network.Send(req);
	}

	private void button_exit_Click(object sender, EventArgs e)
	{
		LeaveRoomReq req = new LeaveRoomReq();
		_network.Send(req);
	}
	#endregion

	#region ui

	public void SetPlayers(List<(ulong, string)> list)
	{
		List<Portrait> portraits = new();

		flowLayoutPanel_players.SuspendLayout();

		foreach (var (id, name) in list)
		{
			if (_players.TryAdd(id, name))
			{
				var portrait = new Portrait(name, id);
				portrait.Name = $"Portrait_{id}";

				portraits.Add(portrait);
			}
		}

		flowLayoutPanel_players.Controls.AddRange(portraits.ToArray());
		flowLayoutPanel_players.ResumeLayout(true);
	}

	private void UpdateBubble(object? sender, EventArgs e)
	{
		List<Control> list = new List<Control>();
		int addCnt = 0;

		while (_controls.TryDequeue(out var bubble))
		{
			list.Add(bubble);
			addCnt++;

			if (addCnt > 50)
			{
				break;
			}
		}

		if (list.Count > 0)
		{
			flowLayoutPanel_main.SuspendLayout();
			flowLayoutPanel_main.Controls.AddRange(list.ToArray());

			while (flowLayoutPanel_main.Controls.Count > MAX_BUBBLE_COUNT)
			{
				var old = flowLayoutPanel_main.Controls[0];
				flowLayoutPanel_main.Controls.RemoveAt(0);
				old.Dispose();
			}

			flowLayoutPanel_main.ResumeLayout(true);

			if (flowLayoutPanel_main.Controls.Count > 0)
			{
				var last = flowLayoutPanel_main.Controls[^1];
				flowLayoutPanel_main.ScrollControlIntoView(last);
			}
		}
	}


	private void AddSystemMsg(string msg)
	{
		var systemMsg = new SystemMsg(msg);
		_controls.Enqueue(systemMsg);
	}

	private void AddBubble(string name, ulong id, string msg)
	{
		var bubble = new Bubble(name, id, msg);
		_controls.Enqueue(bubble);
	}

	private void AddMyBubble(string msg)
	{
		var bubble = new MyBubble(msg);
		_controls.Enqueue(bubble);
	}

	private void AddPlayerItem(ulong id, string name)
	{
		this.DrawUI(() =>
		{
			var portrait = new Portrait(name, id);
			portrait.Name = $"Portrait_{id}";
			flowLayoutPanel_players.Controls.Add(portrait);
		});
	}

	private void RemovePlayerItem(ulong id)
	{
		this.DrawUI(() =>
		{
			var portrait = flowLayoutPanel_players.Controls[$"Portrait_{id}"];
			if (portrait != null)
			{
				flowLayoutPanel_players.Controls.Remove(portrait);
				portrait.Dispose();
			}
		});
	}

	#endregion

	#region network
	private void OnReceived(ISession session, ushort packetId, ReadOnlySpan<byte> body)
	{
		switch ((PacketID)packetId)
		{
			case PacketID.ChatRes:
				{
					ChatRes? res = PacketHelper.Deserialize<ChatRes>(packetId, body);
					Debug.Assert(res != null);
					OnReceived(session, res);
				}
				break;
			case PacketID.LeaveRoomRes:
				{
					LeaveRoomRes? res = PacketHelper.Deserialize<LeaveRoomRes>(packetId, body);
					Debug.Assert(res != null);
					OnReceived(session, res);
				}
				break;

			case PacketID.ChatNot:
				{
					ChatNot? noti = PacketHelper.Deserialize<ChatNot>(packetId, body);
					Debug.Assert(noti != null);
					OnReceived(session, noti);
				}
				break;
			case PacketID.JoinRoomNot:
				{
					JoinRoomNot? noti = PacketHelper.Deserialize<JoinRoomNot>(packetId, body);
					Debug.Assert(noti != null);
					OnReceived(session, noti);
				}
				break;
			case PacketID.LeaveRoomNot:
				{
					LeaveRoomNot? noti = PacketHelper.Deserialize<LeaveRoomNot>(packetId, body);
					Debug.Assert(noti != null);
					OnReceived(session, noti);
				}
				break;
		}
	}

	private void OnReceived(ISession session, ChatNot noti)
	{
		if (_players.TryGetValue(noti.SessionId, out string? name))
		{
			AddBubble(name, noti.SessionId, noti.Desc);
		}
	}

	private void OnReceived(ISession session, JoinRoomNot noti)
	{
		if (_players.TryAdd(noti.Player.SessionId, noti.Player.Name))
		{
			AddPlayerItem(noti.Player.SessionId, noti.Player.Name);
			AddSystemMsg($"{noti.Player.Name}({noti.Player.SessionId})님이 입장했습니다.");
		}
	}

	private void OnReceived(ISession session, LeaveRoomNot noti)
	{
		if (_players.TryRemove(noti.SessionId, out string? name))
		{
			RemovePlayerItem(noti.SessionId);
			AddSystemMsg($"{name}({noti.SessionId})님이 퇴장했습니다.");
		}
	}

	private void OnReceived(ISession session, ChatRes res)
	{
		if (res.ErrorCode != ErrorCode.Ok)
		{
			this.DrawUI(() =>
			{
				MessageBox.Show($"{res.ErrorCode}", "서버 에러");
			});
			return;
		}

		AddMyBubble(res.Desc);
	}

	private void OnReceived(ISession session, LeaveRoomRes res)
	{
		if (res.ErrorCode != ErrorCode.Ok)
		{
			this.DrawUI(() =>
			{
				MessageBox.Show($"{res.ErrorCode}", "서버 에러");
			});
			return;
		}

		this.DrawUI(() =>
		{
			OpenWinLobby?.Invoke();
			this.Close();
		});
	}
	#endregion
}
