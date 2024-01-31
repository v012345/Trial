namespace dictationApp
{
    partial class Form1
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
            if (disposing && (components != null))
            {
                components.Dispose();
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
            BtnPlay = new Button();
            TBoxHiragana = new TextBox();
            BtnPlayAgain = new Button();
            SuspendLayout();
            // 
            // BtnPlay
            // 
            BtnPlay.Location = new Point(319, 528);
            BtnPlay.Name = "BtnPlay";
            BtnPlay.Size = new Size(199, 58);
            BtnPlay.TabIndex = 1;
            BtnPlay.Text = "播放";
            BtnPlay.UseVisualStyleBackColor = true;
            BtnPlay.Click += BtnPlay_Click;
            // 
            // TBoxHiragana
            // 
            TBoxHiragana.Font = new Font("Segoe UI", 12F);
            TBoxHiragana.Location = new Point(12, 12);
            TBoxHiragana.Multiline = true;
            TBoxHiragana.Name = "TBoxHiragana";
            TBoxHiragana.Size = new Size(301, 575);
            TBoxHiragana.TabIndex = 0;
            TBoxHiragana.TextChanged += textBox1_TextChanged_1;
            // 
            // BtnPlayAgain
            // 
            BtnPlayAgain.Location = new Point(319, 457);
            BtnPlayAgain.Name = "BtnPlayAgain";
            BtnPlayAgain.RightToLeft = RightToLeft.No;
            BtnPlayAgain.Size = new Size(199, 65);
            BtnPlayAgain.TabIndex = 2;
            BtnPlayAgain.Text = "再次播放";
            BtnPlayAgain.UseVisualStyleBackColor = true;
            BtnPlayAgain.Click += BtnPlayAgain_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(524, 599);
            Controls.Add(BtnPlayAgain);
            Controls.Add(TBoxHiragana);
            Controls.Add(BtnPlay);
            Name = "Form1";
            Text = "Form1";
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button BtnPlay;
        private TextBox TBoxHiragana;
        private Button BtnPlayAgain;
    }
}
