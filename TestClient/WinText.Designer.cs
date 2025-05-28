namespace TestClient
{
	partial class WinText
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing)
			{
				if (_network != null)
				{
					_network.NetReceivedCallback -= OnReceived;
				}

				OpenWinLobby = null;

				if (components != null)
				{
					components.Dispose();
				}
			}

			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			richTextBox_chat = new RichTextBox();
			button_exit = new Button();
			button_enter = new Button();
			textBox_msg = new TextBox();
			SuspendLayout();
			// 
			// richTextBox_chat
			// 
			richTextBox_chat.BackColor = SystemColors.Window;
			richTextBox_chat.BorderStyle = BorderStyle.None;
			richTextBox_chat.Location = new Point(12, 41);
			richTextBox_chat.Name = "richTextBox_chat";
			richTextBox_chat.ReadOnly = true;
			richTextBox_chat.ScrollBars = RichTextBoxScrollBars.Vertical;
			richTextBox_chat.Size = new Size(400, 374);
			richTextBox_chat.TabIndex = 0;
			richTextBox_chat.Text = "";
			// 
			// button_exit
			// 
			button_exit.Location = new Point(12, 12);
			button_exit.Name = "button_exit";
			button_exit.Size = new Size(75, 23);
			button_exit.TabIndex = 1;
			button_exit.Text = "나가기";
			button_exit.UseVisualStyleBackColor = true;
			button_exit.Click += button_exit_Click;
			// 
			// button_enter
			// 
			button_enter.Location = new Point(328, 435);
			button_enter.Name = "button_enter";
			button_enter.Size = new Size(75, 23);
			button_enter.TabIndex = 2;
			button_enter.Text = "보내기";
			button_enter.UseVisualStyleBackColor = true;
			button_enter.Click += button_enter_Click;
			// 
			// textBox_msg
			// 
			textBox_msg.Location = new Point(16, 436);
			textBox_msg.Name = "textBox_msg";
			textBox_msg.Size = new Size(306, 23);
			textBox_msg.TabIndex = 3;
			// 
			// WinText
			// 
			AutoScaleDimensions = new SizeF(7F, 15F);
			AutoScaleMode = AutoScaleMode.Font;
			ClientSize = new Size(424, 480);
			Controls.Add(textBox_msg);
			Controls.Add(button_enter);
			Controls.Add(button_exit);
			Controls.Add(richTextBox_chat);
			FormBorderStyle = FormBorderStyle.FixedSingle;
			MaximizeBox = false;
			Name = "WinText";
			StartPosition = FormStartPosition.Manual;
			Text = "채팅";
			ResumeLayout(false);
			PerformLayout();
		}

		#endregion

		private RichTextBox richTextBox_chat;
		private Button button_exit;
		private Button button_enter;
		private TextBox textBox_msg;
	}
}