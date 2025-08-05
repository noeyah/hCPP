using hCSharpLibrary.Network;
using Packet;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;

namespace TestClient;

public partial class WinText : Form
{
	public event Action? OpenWinLobby;
	private Network _network;

	private ConcurrentDictionary<ulong, string> _players = new();

	public WinText(Network network)
	{
		_network = network;
		_network.NetReceivedCallback += OnReceived;

		InitializeComponent();

		AddChat("나", 0, $"입장했습니다.", Color.Blue);
	}

	public void SetPlayers(List<(ulong, string)> list)
	{
		foreach (var (id, name) in list)
		{
			_players.TryAdd(id, name);
		}
	}

	private void button_exit_Click(object sender, EventArgs e)
	{
		LeaveRoomReq req = new LeaveRoomReq();
		_network.Send(req);
	}

	private void button_enter_Click(object sender, EventArgs e)
	{
		if (string.IsNullOrWhiteSpace(textBox_msg.Text))
		{
			textBox_msg.Clear();
			return;
		}

		if (textBox_msg.Text.Length > (int)Defaults.MessageMaxLimit)
		{
			MessageBox.Show("문자 길이 초과", "에러");
			return;
		}

		ChatReq req = new ChatReq();
		req.Desc = textBox_msg.Text;
		textBox_msg.Clear();

		_network.Send(req);
	}

	private void AddChat(string name, ulong id, string desc, Color color)
	{
		this.DrawUI(() =>
		{
			richTextBox_chat.SelectionStart = richTextBox_chat.TextLength;
			richTextBox_chat.SelectionLength = 0;

			richTextBox_chat.SelectionColor = color;

			if (id != 0)
			{
				richTextBox_chat.AppendText($"{name}({id}) : {desc}" + Environment.NewLine);
			}
			else
			{
				richTextBox_chat.AppendText($"{name} : {desc}" + Environment.NewLine);
			}

			richTextBox_chat.ScrollToCaret();
		});
	}

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
			AddChat(name, noti.SessionId, noti.Desc, Color.Black);
		}
	}

	private void OnReceived(ISession session, JoinRoomNot noti)
	{
		if (_players.TryAdd(noti.Player.SessionId, noti.Player.Name))
		{
			AddChat(noti.Player.Name, noti.Player.SessionId, $"입장했습니다.", Color.Blue);
		}
	}

	private void OnReceived(ISession session, LeaveRoomNot noti)
	{
		if (_players.TryRemove(noti.SessionId, out string? name))
		{
			AddChat(name, noti.SessionId, $"퇴장했습니다.", Color.Red);
		}
	}

	private void OnReceived(ISession session, ChatRes res)
	{
		if (res.ErrorCode != ErrorCode.Ok)
		{
			this.ShowMsgBox($"{res.ErrorCode}", "서버 에러");
			return;
		}

		AddChat("나", 0, res.Desc, Color.Purple);
	}

	private void OnReceived(ISession session, LeaveRoomRes res)
	{
		if (res.ErrorCode != ErrorCode.Ok)
		{
			this.ShowMsgBox($"{res.ErrorCode}", "서버 에러");
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
