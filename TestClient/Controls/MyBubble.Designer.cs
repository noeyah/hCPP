namespace TestClient
{
	partial class MyBubble
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
			label_chat = new Label();
			SuspendLayout();
			// 
			// label_chat
			// 
			label_chat.AutoSize = true;
			label_chat.BackColor = SystemColors.Info;
			label_chat.Font = new Font("맑은 고딕", 10F);
			label_chat.Location = new Point(15, 0);
			label_chat.Margin = new Padding(3);
			label_chat.MaximumSize = new Size(300, 0);
			label_chat.Name = "label_chat";
			label_chat.Padding = new Padding(5);
			label_chat.Size = new Size(80, 29);
			label_chat.TabIndex = 0;
			label_chat.Text = "나의 채팅";
			label_chat.TextAlign = ContentAlignment.TopRight;
			// 
			// MyBubble
			// 
			AutoScaleDimensions = new SizeF(7F, 15F);
			AutoScaleMode = AutoScaleMode.Font;
			AutoSize = true;
			AutoSizeMode = AutoSizeMode.GrowAndShrink;
			BackColor = SystemColors.GradientInactiveCaption;
			Controls.Add(label_chat);
			Name = "MyBubble";
			Size = new Size(98, 32);
			ResumeLayout(false);
			PerformLayout();
		}

		#endregion

		private Label label_chat;
	}
}
