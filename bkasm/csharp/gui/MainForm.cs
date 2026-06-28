using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace BkasmGui
{
    public class MainForm : Form
    {
        private TextBox txtFile;
        private Button btnBrowse;
        private Button btnCompile;
        private TextBox txtOutput;
        private OpenFileDialog openFileDialog;

        public MainForm()
        {
            Text = "BKASM Assembler";
            Size = new Size(FormHints.FormWidth, FormHints.FormHeight);
            MinimumSize = new Size(FormHints.MinWidth, FormHints.MinHeight);
            StartPosition = FormStartPosition.CenterScreen;

            var lblFile = new Label
            {
                Text = "Source file:",
                Location = new Point(FormHints.Pad, FormHints.LabelY),
                Size = new Size(FormHints.LabelWidth, FormHints.LabelHeight)
            };

            txtFile = new TextBox
            {
                Location = new Point(FormHints.FileTextX, FormHints.FileTextY),
                Width = FormHints.FileTextWidth,
                ReadOnly = true,
                Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right
            };

            btnBrowse = new Button();
            btnBrowse.Text = "Browse...";
            btnBrowse.Location = new Point(FormHints.BrowseX, FormHints.BrowseY);
            btnBrowse.Width = FormHints.BrowseWidth;
            btnBrowse.Anchor = AnchorStyles.Top | AnchorStyles.Right;
            btnBrowse.Click += BtnBrowse_Click;

            btnCompile = new Button();
            btnCompile.Text = "Compile";
            btnCompile.Location = new Point(FormHints.Pad, FormHints.CompileY);
            btnCompile.Width = FormHints.CompileWidth;
            btnCompile.Click += BtnCompile_Click;

            txtOutput = new TextBox
            {
                Location = new Point(FormHints.Pad, FormHints.OutputY),
                Width = FormHints.OutputWidth,
                Height = FormHints.OutputHeight,
                Multiline = true,
                ReadOnly = true,
                ScrollBars = ScrollBars.Vertical,
                Font = new Font(FormHints.ConsoleFontFamily, FormHints.ConsoleFontSize),
                BackColor = Color.FromArgb(FormHints.OutputBgR, FormHints.OutputBgG, FormHints.OutputBgB),
                ForeColor = Color.Lime,
                Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right
            };

            openFileDialog = new OpenFileDialog
            {
                Filter = "Assembly files (*.asm)|*.asm|All files (*.*)|*.*",
                Title = "Select an assembly source file"
            };

            Controls.Add(lblFile);
            Controls.Add(txtFile);
            Controls.Add(btnBrowse);
            Controls.Add(btnCompile);
            Controls.Add(txtOutput);
        }

        private void BtnBrowse_Click(object sender, EventArgs e)
        {
            if (openFileDialog.ShowDialog() == DialogResult.OK)
                txtFile.Text = openFileDialog.FileName;
        }

        private void BtnCompile_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(txtFile.Text))
            {
                txtOutput.Text = "Please select a file first.";
                return;
            }

            if (!File.Exists(txtFile.Text))
            {
                txtOutput.Text = "File not found: " + txtFile.Text;
                return;
            }

            btnCompile.Enabled = false;
            txtOutput.Text = "Compiling...\r\n";

            try
            {
                string bkasmPath = Path.Combine(Application.StartupPath, "bkasm.exe");
                if (!File.Exists(bkasmPath))
                    bkasmPath = Path.Combine(Directory.GetCurrentDirectory(), "bkasm.exe");

                string outFile = Path.ChangeExtension(txtFile.Text, ".bin");

                var psi = new ProcessStartInfo
                {
                    FileName = "mono",
                    Arguments = $"\"{bkasmPath}\" \"{txtFile.Text}\" \"{outFile}\"",
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    UseShellExecute = false,
                    CreateNoWindow = true
                };

                using (var proc = new Process { StartInfo = psi })
                {
                    proc.Start();
                    string stdout = proc.StandardOutput.ReadToEnd();
                    string stderr = proc.StandardError.ReadToEnd();
                    proc.WaitForExit();

                    string result = "";
                    if (!string.IsNullOrEmpty(stdout))
                        result += stdout;
                    if (!string.IsNullOrEmpty(stderr))
                        result += stderr;

                    if (proc.ExitCode == 0)
                    {
                        result += $"\r\nOutput file: {outFile}";
                        if (File.Exists(outFile))
                            result += $" ({new FileInfo(outFile).Length} bytes)";
                        result += "\r\nCompilation successful.";
                    }
                    else
                    {
                        result += $"\r\nCompilation failed (exit code {proc.ExitCode}).";
                    }

                    txtOutput.Text = result;
                }
            }
            catch (Exception ex)
            {
                txtOutput.Text = "Error: " + ex.Message;
            }
            finally
            {
                btnCompile.Enabled = true;
            }
        }
    }
}
