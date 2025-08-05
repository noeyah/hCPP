namespace TestClient;

public abstract class ChatEvent
{
}

public class UserJoinEvent : ChatEvent
{
	public ulong uid;
	public string Name;

	public UserJoinEvent(ulong uid, string name)
	{
		this.uid = uid;
		this.Name = name;
	}
}

public class UserLeftEvent : ChatEvent
{
	public ulong uid;
	public string Name;

	public UserLeftEvent(ulong uid, string name)
	{
		this.uid = uid;
		this.Name = name;
	}
}

public class ChatMessageEvent : ChatEvent
{
	public ulong uid;
	public string Name;
	public string Message;
	public bool IsMine => uid == 0;

	public ChatMessageEvent(string message)
	{
		this.uid = 0;
		this.Name = string.Empty;
		this.Message = message;
	}

	public ChatMessageEvent(ulong uid, string name, string message)
	{
		this.uid = uid;
		this.Name = name;
		this.Message = message;
	}
}

public class SelfJoinEvent : ChatEvent
{
}

public class User
{
	public ulong uid;
	public string Name;
	public User(ulong uid, string name)
	{
		this.uid = uid;
		this.Name = name;
	}
}