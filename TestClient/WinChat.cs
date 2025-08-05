using Google.Protobuf;
using hCSharpLibrary.Network;
using Packet;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Xml.Linq;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Header;

namespace TestClient;

public partial class WinChat : Form
{
	public event Action? OpenWinLobby;
	private Network _network;

	private ConcurrentDictionary<ulong, User> _dicUser = new();
	private ConcurrentQueue<ChatEvent> _eventQueue = new();
	private int _isEventUpdate = 0;

	private readonly Font _font;
	private readonly Padding _bubblePadding = new Padding(12, 10, 12, 10);
	private const int ITEM_VERTICA_MARGIN = 5;
	private const int MAX_BUBBLE_WIDTH = 300;

	public WinChat(Network network)
	{
		_network = network;
		_network.NetReceivedCallback += OnReceived;

		_font = new Font("맑은 고딕", 9);
		InitializeComponent();

		listBox_chat.DrawMode = DrawMode.OwnerDrawVariable;
		listBox_chat.MeasureItem += ListBoxChat_MeasureItem;
		listBox_chat.DrawItem += ListBoxChat_DrawItem;

		listBox_user.DrawMode = DrawMode.OwnerDrawFixed;
		listBox_user.ItemHeight = 70;
		listBox_user.DrawItem += ListBoxUser_DrawItem;

		SelfJoinEvent selfEvent = new SelfJoinEvent();
		listBox_chat.Items.Add(selfEvent);
	}

	public void InitUsers(List<(ulong, string)> list)
	{
		var users = list.Select(v => new User(v.Item1, v.Item2));

		_dicUser.Clear();
		foreach (var user in users)
		{
			_dicUser.TryAdd(user.uid, user);
		}

		listBox_user.SuspendLayout();

		listBox_user.Items.Clear();
		listBox_user.Items.AddRange(_dicUser.Values.ToArray());

		listBox_user.ResumeLayout();
	}

	private void PushChatEvent(ChatEvent chatEvent)
	{
		_eventQueue.Enqueue(chatEvent);

		if (Interlocked.CompareExchange(ref _isEventUpdate, 1, 0) == 0)
		{
			this.BeginInvoke(ProcessChatEvent);
		}
	}

	private void ProcessChatEvent()
	{
		while (_eventQueue.TryDequeue(out ChatEvent? chatEvent))
		{
			listBox_chat.Items.Add(chatEvent);
		}

		listBox_chat.TopIndex = listBox_chat.Items.Count - 1;

		Interlocked.Exchange(ref _isEventUpdate, 0);

		if (!_eventQueue.IsEmpty)
		{
			if (Interlocked.CompareExchange(ref _isEventUpdate, 1, 0) == 0)
			{
				this.BeginInvoke(ProcessChatEvent);
			}
		}
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


	private void ListBoxChat_MeasureItem(object? sender, MeasureItemEventArgs e)
	{
		if (e.Index < 0)
			return;

		ChatEvent chatEvent = (ChatEvent)listBox_chat.Items[e.Index];

		int contentHeight = 0;

		switch (chatEvent)
		{
			case UserJoinEvent joinEvent:
			case UserLeftEvent leftEvent:
			case SelfJoinEvent selfEvent:
				contentHeight = 20;
				break;
			case ChatMessageEvent msgEvent:
				{
					int maxTextWidth = MAX_BUBBLE_WIDTH - _bubblePadding.Horizontal;
					SizeF textSize = e.Graphics.MeasureString(msgEvent.Message, listBox_chat.Font, maxTextWidth);
					contentHeight = (int)Math.Ceiling(textSize.Height) + _bubblePadding.Vertical;

					if (!msgEvent.IsMine)
					{
						bool showName = true;
						if (e.Index - 1 >= 0)
						{
							var prevChatEvent = (ChatEvent)listBox_chat.Items[e.Index - 1];
							if (prevChatEvent is ChatMessageEvent prevMsgEvent)
							{
								if (prevMsgEvent.uid == msgEvent.uid)
								{
									showName = false;
								}
							}
						}

						if (showName)
						{
							contentHeight += listBox_chat.Font.Height + 4;
						}
					}
				}
				break;
			default:
				contentHeight = 10;
				break;
		}

		e.ItemHeight = contentHeight + ITEM_VERTICA_MARGIN;
	}

	private void ListBoxChat_DrawItem(object? sender, DrawItemEventArgs e)
	{
		if (e.Index < 0)
			return;


		ChatEvent chatEvent = (ChatEvent)listBox_chat.Items[e.Index];

		e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
		e.Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

		//e.DrawBackground();
		e.Graphics.FillRectangle(Brushes.LightSteelBlue, e.Bounds);

		switch (chatEvent)
		{
			case UserJoinEvent joinEvent:
				{
					string msg = $"{joinEvent.Name}({joinEvent.uid})님이 입장했습니다.";
					TextRenderer.DrawText(e.Graphics, msg, _font, e.Bounds, Color.DimGray, TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter);
				}
				break;
			case UserLeftEvent leftEvent:
				{
					string msg = $"{leftEvent.Name}({leftEvent.uid})님이 퇴장했습니다.";
					TextRenderer.DrawText(e.Graphics, msg, _font, e.Bounds, Color.DimGray, TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter);
				}
				break;
			case ChatMessageEvent msgEvent:
				{
					int maxTextWidth = MAX_BUBBLE_WIDTH - _bubblePadding.Horizontal;
					SizeF textSize = e.Graphics.MeasureString(msgEvent.Message, listBox_chat.Font, maxTextWidth);

					Size bubbleSize = new Size(
						(int)Math.Ceiling(textSize.Width) + 1 + _bubblePadding.Horizontal,
						(int)Math.Ceiling(textSize.Height) + _bubblePadding.Vertical);

					Rectangle bubbleRect;

					if (msgEvent.IsMine)
					{
						bubbleRect = new Rectangle(
							e.Bounds.Right - bubbleSize.Width - 5,
							e.Bounds.Top + 5,
							bubbleSize.Width,
							bubbleSize.Height);
					}
					else
					{
						bool showName = true;
						if (e.Index - 1 >= 0)
						{
							var prevChatEvent = (ChatEvent)listBox_chat.Items[e.Index - 1];
							if (prevChatEvent is ChatMessageEvent prevMsgEvent)
							{
								if (prevMsgEvent.uid == msgEvent.uid)
								{
									showName = false;
								}
							}
						}

						int rectY = 0;
						if (showName)
						{
							Point namePoint = new Point(e.Bounds.Left + 5, e.Bounds.Top + 5);
							e.Graphics.DrawString($"{msgEvent.Name}({msgEvent.uid})", listBox_chat.Font, Brushes.Black, namePoint);

							rectY = namePoint.Y + listBox_chat.Font.Height + 4;
						}
						else
						{
							rectY = e.Bounds.Top + 5;
						}

						bubbleRect = new Rectangle(
							e.Bounds.Left + 5,
							rectY,
							bubbleSize.Width,
							bubbleSize.Height);
					}

					Rectangle textRect = new Rectangle(
						bubbleRect.Left + _bubblePadding.Left,
						bubbleRect.Top + _bubblePadding.Top,
						(int)Math.Ceiling(textSize.Width),
						(int)Math.Ceiling(textSize.Height));

					e.Graphics.FillRectangle(msgEvent.IsMine ? Brushes.Gold : Brushes.White, bubbleRect);
					e.Graphics.DrawString(msgEvent.Message, listBox_chat.Font, Brushes.Black, textRect);
				}
				break;
			case SelfJoinEvent selfEvent:
				{
					string msg = "입장했습니다.";
					TextRenderer.DrawText(e.Graphics, msg, new Font("맑은 고딕", 9), e.Bounds, Color.DimGray, TextFormatFlags.HorizontalCenter | TextFormatFlags.VerticalCenter);
				}
				break;
			default:
				break;
		}
	}


	private void ListBoxUser_DrawItem(object? sender, DrawItemEventArgs e)
	{
		if (e.Index < 0)
			return;

		//e.DrawBackground();
		e.Graphics.FillRectangle(Brushes.White, e.Bounds);

		User user = (User)listBox_user.Items[e.Index];

		var profileRect = new Rectangle(e.Bounds.Left + 5, e.Bounds.Top, 70, 70);
		var profileImage = GetImage(user.uid);
		if (profileImage != null)
		{
			e.Graphics.DrawImage(profileImage, profileRect);
		}
		else
		{
			e.Graphics.FillRectangle(Brushes.White, profileRect);
		}

		var nameRect = new Rectangle(
			profileRect.Right + 10,
			e.Bounds.Top,
			e.Bounds.Width - profileRect.Right - 15,
			e.Bounds.Height
			);

		TextFormatFlags flags = TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.WordBreak;
		TextRenderer.DrawText(e.Graphics, $"{user.Name}({user.uid})", e.Font, nameRect, Color.Black, flags);
	}

	Bitmap? GetImage(ulong uid)
	{
		var num = uid % 5;
		Bitmap? bitmap = null;

		switch (num)
		{
			case 0: bitmap = Properties.Resources.bear; break;
			case 1: bitmap = Properties.Resources.cat; break;
			case 2: bitmap = Properties.Resources.chicken; break;
			case 3: bitmap = Properties.Resources.lion; break;
			case 4: bitmap = Properties.Resources.panda; break;
			default:
				break;
		}

		return bitmap;
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
		if (_dicUser.TryGetValue(noti.SessionId, out var user))
		{
			ChatMessageEvent msgEvent = new ChatMessageEvent(noti.SessionId, user.Name, noti.Desc);
			PushChatEvent(msgEvent);
		}
	}

	private void OnReceived(ISession session, JoinRoomNot noti)
	{
		var newUser = new User(noti.Player.SessionId, noti.Player.Name);
		if (_dicUser.TryAdd(noti.Player.SessionId, newUser))
		{
			this.DrawUI(() =>
			{
				listBox_user.Items.Add(newUser);
			});

			UserJoinEvent joinEvent = new UserJoinEvent(noti.Player.SessionId, noti.Player.Name);
			PushChatEvent(joinEvent);
		}
	}

	private void OnReceived(ISession session, LeaveRoomNot noti)
	{
		if (_dicUser.TryRemove(noti.SessionId, out var user))
		{
			this.DrawUI(() =>
			{
				listBox_user.Items.Remove(user);
			});

			UserLeftEvent leftEvent = new UserLeftEvent(noti.SessionId, user.Name);
			PushChatEvent(leftEvent);
		}
	}

	private void OnReceived(ISession session, ChatRes res)
	{
		if (res.ErrorCode != ErrorCode.Ok)
		{
			this.ShowMsgBox($"{res.ErrorCode}", "서버 에러");
			return;
		}

		ChatMessageEvent msgEvent = new ChatMessageEvent(res.Desc);
		PushChatEvent(msgEvent);
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
