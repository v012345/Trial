namespace Client
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
            BtnNextStep = new Button();
            button2 = new Button();
            radioButton1 = new RadioButton();
            radioButton2 = new RadioButton();
            radioButton3 = new RadioButton();
            radioButton4 = new RadioButton();
            radioButton5 = new RadioButton();
            LblConnectionState = new Label();
            BtnConnectServer = new Button();
            SuspendLayout();
            // 
            // BtnNextStep
            // 
            BtnNextStep.Location = new Point(630, 409);
            BtnNextStep.Name = "BtnNextStep";
            BtnNextStep.Size = new Size(158, 29);
            BtnNextStep.TabIndex = 1;
            BtnNextStep.Text = "下一步";
            BtnNextStep.UseVisualStyleBackColor = true;
            BtnNextStep.Click += BtnNextStep_Click;
            // 
            // button2
            // 
            button2.Location = new Point(450, 409);
            button2.Name = "button2";
            button2.Size = new Size(174, 29);
            button2.TabIndex = 2;
            button2.Text = "button2";
            button2.UseVisualStyleBackColor = true;
            // 
            // radioButton1
            // 
            radioButton1.AutoSize = true;
            radioButton1.Location = new Point(12, 43);
            radioButton1.Name = "radioButton1";
            radioButton1.Size = new Size(117, 24);
            radioButton1.TabIndex = 4;
            radioButton1.TabStop = true;
            radioButton1.Text = "radioButton1";
            radioButton1.UseVisualStyleBackColor = true;
            // 
            // radioButton2
            // 
            radioButton2.AutoSize = true;
            radioButton2.Location = new Point(12, 73);
            radioButton2.Name = "radioButton2";
            radioButton2.Size = new Size(117, 24);
            radioButton2.TabIndex = 5;
            radioButton2.TabStop = true;
            radioButton2.Text = "radioButton2";
            radioButton2.UseVisualStyleBackColor = true;
            // 
            // radioButton3
            // 
            radioButton3.AutoSize = true;
            radioButton3.Location = new Point(12, 103);
            radioButton3.Name = "radioButton3";
            radioButton3.Size = new Size(117, 24);
            radioButton3.TabIndex = 6;
            radioButton3.TabStop = true;
            radioButton3.Text = "radioButton3";
            radioButton3.UseVisualStyleBackColor = true;
            // 
            // radioButton4
            // 
            radioButton4.AutoSize = true;
            radioButton4.Location = new Point(12, 133);
            radioButton4.Name = "radioButton4";
            radioButton4.Size = new Size(117, 24);
            radioButton4.TabIndex = 7;
            radioButton4.TabStop = true;
            radioButton4.Text = "radioButton4";
            radioButton4.UseVisualStyleBackColor = true;
            // 
            // radioButton5
            // 
            radioButton5.AutoSize = true;
            radioButton5.Location = new Point(12, 163);
            radioButton5.Name = "radioButton5";
            radioButton5.Size = new Size(117, 24);
            radioButton5.TabIndex = 8;
            radioButton5.TabStop = true;
            radioButton5.Text = "radioButton5";
            radioButton5.UseVisualStyleBackColor = true;
            // 
            // LblConnectionState
            // 
            LblConnectionState.AutoSize = true;
            LblConnectionState.ForeColor = Color.Crimson;
            LblConnectionState.Location = new Point(12, 383);
            LblConnectionState.Name = "LblConnectionState";
            LblConnectionState.Size = new Size(57, 20);
            LblConnectionState.TabIndex = 9;
            LblConnectionState.Text = "已断开";
            LblConnectionState.Click += LblConnectionState_Click;
            // 
            // BtnConnectServer
            // 
            BtnConnectServer.Location = new Point(12, 406);
            BtnConnectServer.Name = "BtnConnectServer";
            BtnConnectServer.Size = new Size(117, 29);
            BtnConnectServer.TabIndex = 10;
            BtnConnectServer.Text = "连接服务器";
            BtnConnectServer.UseVisualStyleBackColor = true;
            BtnConnectServer.Click += BtnConnectServer_Click;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(BtnConnectServer);
            Controls.Add(LblConnectionState);
            Controls.Add(radioButton5);
            Controls.Add(radioButton4);
            Controls.Add(radioButton3);
            Controls.Add(radioButton2);
            Controls.Add(radioButton1);
            Controls.Add(button2);
            Controls.Add(BtnNextStep);
            Name = "Form1";
            Text = "Form1";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Button BtnNextStep;
        private Button button2;
        private RadioButton radioButton1;
        private RadioButton radioButton2;
        private RadioButton radioButton3;
        private RadioButton radioButton4;
        private RadioButton radioButton5;
        private Label LblConnectionState;
        private Button BtnConnectServer;
    }
}
