
namespace TestClient
{
	public partial class MyBubble : UserControl
	{
		public MyBubble(string msg)
		{
			InitializeComponent();

			int right_pos = 360;
			label_chat.Text = msg;
			Size textSize = TextRenderer.MeasureText(label_chat.Text, label_chat.Font);
			label_chat.Size = new Size(textSize.Width, label_chat.Height);
			label_chat.Left = right_pos - label_chat.Width;
		}
	}
}
