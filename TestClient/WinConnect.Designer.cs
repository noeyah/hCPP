namespace TestClient
{
	partial class WinConnect
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
					_network.NetConnectedCallback -= OnConnected;
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
			label_ip = new Label();
			textBox_ip = new TextBox();
			label_port = new Label();
			textBox_port = new TextBox();
			button_connect = new Button();
			SuspendLayout();
			// 
			// label_ip
			// 
			label_ip.AutoSize = true;
			label_ip.Location = new Point(36, 85);
			label_ip.Name = "label_ip";
			label_ip.Size = new Size(17, 15);
			label_ip.TabIndex = 0;
			label_ip.Text = "IP";
			// 
			// textBox_ip
			// 
			textBox_ip.Location = new Point(59, 82);
			textBox_ip.Name = "textBox_ip";
			textBox_ip.Size = new Size(118, 23);
			textBox_ip.TabIndex = 1;
			textBox_ip.Text = "127.0.0.1";
			// 
			// label_port
			// 
			label_port.AutoSize = true;
			label_port.Location = new Point(202, 85);
			label_port.Name = "label_port";
			label_port.Size = new Size(29, 15);
			label_port.TabIndex = 2;
			label_port.Text = "Port";
			// 
			// textBox_port
			// 
			textBox_port.Location = new Point(237, 82);
			textBox_port.Name = "textBox_port";
			textBox_port.Size = new Size(100, 23);
			textBox_port.TabIndex = 3;
			textBox_port.Text = "7777";
			// 
			// button_connect
			// 
			button_connect.Location = new Point(140, 151);
			button_connect.Name = "button_connect";
			button_connect.Size = new Size(100, 23);
			button_connect.TabIndex = 4;
			button_connect.Text = "연결";
			button_connect.UseVisualStyleBackColor = true;
			button_connect.Click += button_connect_Click;
			// 
			// WinConnect
			// 
			AutoScaleDimensions = new SizeF(7F, 15F);
			AutoScaleMode = AutoScaleMode.Font;
			ClientSize = new Size(384, 261);
			Controls.Add(button_connect);
			Controls.Add(textBox_port);
			Controls.Add(label_port);
			Controls.Add(textBox_ip);
			Controls.Add(label_ip);
			FormBorderStyle = FormBorderStyle.FixedSingle;
			MaximizeBox = false;
			Name = "WinConnect";
			StartPosition = FormStartPosition.CenterScreen;
			Text = "연결";
			ResumeLayout(false);
			PerformLayout();
		}

		#endregion

		private Label label_ip;
		private TextBox textBox_ip;
		private Label label_port;
		private TextBox textBox_port;
		private Button button_connect;
	}
}