
namespace TestClient
{
	public partial class SystemMsg : UserControl
	{
		public SystemMsg(string msg)
		{
			InitializeComponent();

			label_msg.Text = msg;
		}
	}
}
