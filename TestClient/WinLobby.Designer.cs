namespace TestClient
{
	partial class WinLobby
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

				OpenWinChat = null;
				OpenWinText = null;

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
			label_name = new Label();
			textBox_name = new TextBox();
			button_join = new Button();
			radioButton_chat = new RadioButton();
			radioButton_text = new RadioButton();
			groupBox_room = new GroupBox();
			groupBox_room.SuspendLayout();
			SuspendLayout();
			// 
			// label_name
			// 
			label_name.AutoSize = true;
			label_name.Location = new Point(78, 120);
			label_name.Name = "label_name";
			label_name.Size = new Size(31, 15);
			label_name.TabIndex = 0;
			label_name.Text = "이름";
			// 
			// textBox_name
			// 
			textBox_name.Location = new Point(125, 117);
			textBox_name.MaxLength = 12;
			textBox_name.Name = "textBox_name";
			textBox_name.Size = new Size(165, 23);
			textBox_name.TabIndex = 1;
			// 
			// button_join
			// 
			button_join.Location = new Point(140, 177);
			button_join.Name = "button_join";
			button_join.Size = new Size(100, 23);
			button_join.TabIndex = 2;
			button_join.Text = "입장";
			button_join.UseVisualStyleBackColor = true;
			button_join.Click += button_join_Click;
			// 
			// radioButton_chat
			// 
			radioButton_chat.AutoSize = true;
			radioButton_chat.Checked = true;
			radioButton_chat.Location = new Point(39, 22);
			radioButton_chat.Name = "radioButton_chat";
			radioButton_chat.Size = new Size(112, 19);
			radioButton_chat.TabIndex = 3;
			radioButton_chat.TabStop = true;
			radioButton_chat.Text = "채팅방 UI (기본)";
			radioButton_chat.UseVisualStyleBackColor = true;
			// 
			// radioButton_text
			// 
			radioButton_text.AutoSize = true;
			radioButton_text.Location = new Point(190, 22);
			radioButton_text.Name = "radioButton_text";
			radioButton_text.Size = new Size(61, 19);
			radioButton_text.TabIndex = 4;
			radioButton_text.TabStop = true;
			radioButton_text.Text = "텍스트";
			radioButton_text.UseVisualStyleBackColor = true;
			// 
			// groupBox_room
			// 
			groupBox_room.Controls.Add(radioButton_chat);
			groupBox_room.Controls.Add(radioButton_text);
			groupBox_room.Enabled = false;
			groupBox_room.Location = new Point(39, 33);
			groupBox_room.Name = "groupBox_room";
			groupBox_room.Size = new Size(305, 53);
			groupBox_room.TabIndex = 5;
			groupBox_room.TabStop = false;
			// 
			// WinLobby
			// 
			AutoScaleDimensions = new SizeF(7F, 15F);
			AutoScaleMode = AutoScaleMode.Font;
			ClientSize = new Size(384, 261);
			Controls.Add(groupBox_room);
			Controls.Add(button_join);
			Controls.Add(textBox_name);
			Controls.Add(label_name);
			FormBorderStyle = FormBorderStyle.FixedSingle;
			MaximizeBox = false;
			Name = "WinLobby";
			StartPosition = FormStartPosition.Manual;
			Text = "로비";
			groupBox_room.ResumeLayout(false);
			groupBox_room.PerformLayout();
			ResumeLayout(false);
			PerformLayout();
		}

		#endregion

		private Label label_name;
		private TextBox textBox_name;
		private Button button_join;
		private RadioButton radioButton_chat;
		private RadioButton radioButton_text;
		private GroupBox groupBox_room;
	}
}