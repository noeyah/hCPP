namespace TestClient
{
	partial class SystemMsg
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
			label_msg = new Label();
			SuspendLayout();
			// 
			// label_msg
			// 
			label_msg.Location = new Point(0, 0);
			label_msg.Name = "label_msg";
			label_msg.Size = new Size(360, 23);
			label_msg.TabIndex = 0;
			label_msg.Text = "ddd";
			label_msg.TextAlign = ContentAlignment.MiddleCenter;
			// 
			// SystemMsg
			// 
			AutoScaleDimensions = new SizeF(7F, 15F);
			AutoScaleMode = AutoScaleMode.Font;
			AutoSize = true;
			AutoSizeMode = AutoSizeMode.GrowAndShrink;
			BackColor = SystemColors.GradientInactiveCaption;
			Controls.Add(label_msg);
			MinimumSize = new Size(360, 0);
			MaximumSize = new Size(360, 0);
			Name = "SystemMsg";
			Size = new Size(360, 23);
			ResumeLayout(false);
		}

		#endregion

		private Label label_msg;
	}
}
