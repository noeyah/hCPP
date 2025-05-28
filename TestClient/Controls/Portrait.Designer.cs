namespace TestClient
{
	partial class Portrait
	{
		/// <summary> 
		/// 필수 디자이너 변수입니다.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		/// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region 구성 요소 디자이너에서 생성한 코드

		/// <summary> 
		/// 디자이너 지원에 필요한 메서드입니다. 
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
		/// </summary>
		private void InitializeComponent()
		{
			pictureBox_Image = new PictureBox();
			label_name = new Label();
			((System.ComponentModel.ISupportInitialize)pictureBox_Image).BeginInit();
			SuspendLayout();
			// 
			// pictureBox_Image
			// 
			pictureBox_Image.Dock = DockStyle.Left;
			pictureBox_Image.InitialImage = null;
			pictureBox_Image.Location = new Point(0, 0);
			pictureBox_Image.Name = "pictureBox_Image";
			pictureBox_Image.Size = new Size(70, 70);
			pictureBox_Image.SizeMode = PictureBoxSizeMode.CenterImage;
			pictureBox_Image.TabIndex = 0;
			pictureBox_Image.TabStop = false;
			pictureBox_Image.WaitOnLoad = true;
			// 
			// label_name
			// 
			label_name.AutoSize = true;
			label_name.Location = new Point(72, 18);
			label_name.MaximumSize = new Size(100, 30);
			label_name.Name = "label_name";
			label_name.Size = new Size(91, 30);
			label_name.TabIndex = 1;
			label_name.Text = "일이삼사오육칠팔구십일이";
			label_name.TextAlign = ContentAlignment.MiddleLeft;
			// 
			// Portrait
			// 
			AutoScaleDimensions = new SizeF(7F, 15F);
			AutoScaleMode = AutoScaleMode.Font;
			BackColor = SystemColors.Window;
			Controls.Add(label_name);
			Controls.Add(pictureBox_Image);
			Name = "Portrait";
			Size = new Size(170, 70);
			((System.ComponentModel.ISupportInitialize)pictureBox_Image).EndInit();
			ResumeLayout(false);
			PerformLayout();
		}

		#endregion

		private PictureBox pictureBox_Image;
		private Label label_name;
	}
}
