namespace TestClient
{
    partial class WinChat
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
		///  Required method for Designer support - do not modify
		///  the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			panel_bottom = new Panel();
			button_send = new Button();
			richTextBox_bottom = new RichTextBox();
			panel_info = new Panel();
			listBox_user = new ListBox();
			button_exit = new Button();
			listBox_chat = new ListBox();
			panel_main = new Panel();
			panel_bottom.SuspendLayout();
			panel_info.SuspendLayout();
			panel_main.SuspendLayout();
			SuspendLayout();
			// 
			// panel_bottom
			// 
			panel_bottom.BackColor = SystemColors.ControlLightLight;
			panel_bottom.BackgroundImageLayout = ImageLayout.None;
			panel_bottom.Controls.Add(button_send);
			panel_bottom.Controls.Add(richTextBox_bottom);
			panel_bottom.Location = new Point(199, 409);
			panel_bottom.Name = "panel_bottom";
			panel_bottom.Size = new Size(398, 100);
			panel_bottom.TabIndex = 0;
			// 
			// button_send
			// 
			button_send.BackgroundImageLayout = ImageLayout.Zoom;
			button_send.FlatStyle = FlatStyle.Flat;
			button_send.Location = new Point(339, 3);
			button_send.Name = "button_send";
			button_send.Size = new Size(59, 97);
			button_send.TabIndex = 1;
			button_send.Text = "보내기";
			button_send.UseVisualStyleBackColor = true;
			button_send.Click += button_send_Click;
			// 
			// richTextBox_bottom
			// 
			richTextBox_bottom.BackColor = SystemColors.ControlLightLight;
			richTextBox_bottom.BorderStyle = BorderStyle.None;
			richTextBox_bottom.Location = new Point(3, 3);
			richTextBox_bottom.MaxLength = 500;
			richTextBox_bottom.Name = "richTextBox_bottom";
			richTextBox_bottom.ScrollBars = RichTextBoxScrollBars.Vertical;
			richTextBox_bottom.Size = new Size(327, 94);
			richTextBox_bottom.TabIndex = 0;
			richTextBox_bottom.Text = "";
			// 
			// panel_info
			// 
			panel_info.BackColor = SystemColors.Window;
			panel_info.Controls.Add(listBox_user);
			panel_info.Controls.Add(button_exit);
			panel_info.Dock = DockStyle.Left;
			panel_info.Location = new Point(0, 0);
			panel_info.Name = "panel_info";
			panel_info.Size = new Size(200, 509);
			panel_info.TabIndex = 1;
			// 
			// listBox_user
			// 
			listBox_user.Dock = DockStyle.Fill;
			listBox_user.DrawMode = DrawMode.OwnerDrawFixed;
			listBox_user.FormattingEnabled = true;
			listBox_user.IntegralHeight = false;
			listBox_user.ItemHeight = 15;
			listBox_user.Location = new Point(0, 26);
			listBox_user.Name = "listBox_user";
			listBox_user.SelectionMode = SelectionMode.None;
			listBox_user.Size = new Size(200, 483);
			listBox_user.TabIndex = 2;
			// 
			// button_exit
			// 
			button_exit.BackColor = Color.MistyRose;
			button_exit.Dock = DockStyle.Top;
			button_exit.FlatStyle = FlatStyle.Flat;
			button_exit.Location = new Point(0, 0);
			button_exit.Name = "button_exit";
			button_exit.Size = new Size(200, 26);
			button_exit.TabIndex = 1;
			button_exit.Text = "나가기";
			button_exit.UseVisualStyleBackColor = false;
			button_exit.Click += button_exit_Click;
			// 
			// listBox_chat
			// 
			listBox_chat.BackColor = Color.LightSteelBlue;
			listBox_chat.BorderStyle = BorderStyle.None;
			listBox_chat.Dock = DockStyle.Fill;
			listBox_chat.DrawMode = DrawMode.OwnerDrawVariable;
			listBox_chat.FormattingEnabled = true;
			listBox_chat.IntegralHeight = false;
			listBox_chat.ItemHeight = 15;
			listBox_chat.Location = new Point(0, 0);
			listBox_chat.Name = "listBox_chat";
			listBox_chat.SelectionMode = SelectionMode.None;
			listBox_chat.Size = new Size(397, 411);
			listBox_chat.TabIndex = 0;
			// 
			// panel_main
			// 
			panel_main.BackColor = SystemColors.GradientInactiveCaption;
			panel_main.Controls.Add(listBox_chat);
			panel_main.Dock = DockStyle.Top;
			panel_main.Location = new Point(200, 0);
			panel_main.Name = "panel_main";
			panel_main.Size = new Size(397, 411);
			panel_main.TabIndex = 2;
			// 
			// WinChat
			// 
			AutoScaleDimensions = new SizeF(7F, 15F);
			AutoScaleMode = AutoScaleMode.Font;
			AutoSizeMode = AutoSizeMode.GrowAndShrink;
			ClientSize = new Size(597, 509);
			Controls.Add(panel_main);
			Controls.Add(panel_info);
			Controls.Add(panel_bottom);
			FormBorderStyle = FormBorderStyle.FixedSingle;
			MaximizeBox = false;
			Name = "WinChat";
			StartPosition = FormStartPosition.Manual;
			Text = "채팅";
			panel_bottom.ResumeLayout(false);
			panel_info.ResumeLayout(false);
			panel_main.ResumeLayout(false);
			ResumeLayout(false);
		}

		#endregion

		private Panel panel_bottom;
		private RichTextBox richTextBox_bottom;
		private Button button_send;
		private Panel panel_info;
		private Button button_exit;
		private ListBox listBox_user;
		private ListBox listBox_chat;
		private Panel panel_main;
	}
}
