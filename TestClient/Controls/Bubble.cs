
namespace TestClient
{
	public partial class Bubble : UserControl
	{
		public Bubble(string name, ulong id, string msg)
		{
			InitializeComponent();

			label_name.Text = $"{name}({id})";
			label_chat.Text = msg;
		}
	}
}
