
namespace TestClient
{
	public partial class Portrait : UserControl
	{
		public Portrait(string name, ulong id)
		{
			InitializeComponent();

			label_name.Text = $"{name}({id})";
			SetImage(id);
		}

		void SetImage(ulong id)
		{
			var num = id % 5;
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

			if (bitmap is not null)
			{
				pictureBox_Image.Image = bitmap;
			}
		}
	}
}
