using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TestClient;

internal static class Extension
{
	public static void DrawUI(this Form form, Action action)
	{
		if (form.InvokeRequired)
		{
			form.BeginInvoke(() =>
			{
				action();
			});
		}
		else
		{
			action();
		}
	}

	public static void Reset(this RichTextBox textBox)
	{
		textBox.Clear();
		textBox.ScrollBars = RichTextBoxScrollBars.None;
		textBox.ScrollBars = RichTextBoxScrollBars.Vertical;
	}
}
