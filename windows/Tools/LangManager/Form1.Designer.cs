namespace LangManager
{
    partial class Form1
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
            if (disposing && (components != null))
            {
                components.Dispose();
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
            this.textCocosTool = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.radioMergeTrans = new System.Windows.Forms.RadioButton();
            this.radioExtract = new System.Windows.Forms.RadioButton();
            this.radioPublishModifiedCsd = new System.Windows.Forms.RadioButton();
            this.radioPublishAllCsd = new System.Windows.Forms.RadioButton();
            this.radioPublishPlist = new System.Windows.Forms.RadioButton();
            this.textMergedTrans = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.SuspendLayout();
            // 
            // textCocosTool
            // 
            this.textCocosTool.Location = new System.Drawing.Point(124, 15);
            this.textCocosTool.Name = "textCocosTool";
            this.textCocosTool.Size = new System.Drawing.Size(573, 22);
            this.textCocosTool.TabIndex = 0;
            this.textCocosTool.Text = "C:\\Cocos\\Cocos Studio\\Cocos.Tool.exe";
            this.textCocosTool.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(106, 16);
            this.label1.TabIndex = 1;
            this.label1.Text = "Cocos工具路径";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(559, 404);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(229, 34);
            this.button1.TabIndex = 2;
            this.button1.Text = "执行操作";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // radioMergeTrans
            // 
            this.radioMergeTrans.AutoSize = true;
            this.radioMergeTrans.Location = new System.Drawing.Point(15, 175);
            this.radioMergeTrans.Name = "radioMergeTrans";
            this.radioMergeTrans.Size = new System.Drawing.Size(150, 20);
            this.radioMergeTrans.TabIndex = 3;
            this.radioMergeTrans.TabStop = true;
            this.radioMergeTrans.Text = "合并新翻译到 trans";
            this.radioMergeTrans.UseVisualStyleBackColor = true;
            // 
            // radioExtract
            // 
            this.radioExtract.AutoSize = true;
            this.radioExtract.Location = new System.Drawing.Point(15, 55);
            this.radioExtract.Name = "radioExtract";
            this.radioExtract.Size = new System.Drawing.Size(88, 20);
            this.radioExtract.TabIndex = 4;
            this.radioExtract.TabStop = true;
            this.radioExtract.Text = "提取文字";
            this.radioExtract.UseVisualStyleBackColor = true;
            this.radioExtract.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // radioPublishModifiedCsd
            // 
            this.radioPublishModifiedCsd.AutoSize = true;
            this.radioPublishModifiedCsd.Location = new System.Drawing.Point(15, 85);
            this.radioPublishModifiedCsd.Name = "radioPublishModifiedCsd";
            this.radioPublishModifiedCsd.Size = new System.Drawing.Size(113, 20);
            this.radioPublishModifiedCsd.TabIndex = 5;
            this.radioPublishModifiedCsd.TabStop = true;
            this.radioPublishModifiedCsd.Text = "发布变动 csd";
            this.radioPublishModifiedCsd.UseVisualStyleBackColor = true;
            // 
            // radioPublishAllCsd
            // 
            this.radioPublishAllCsd.AutoSize = true;
            this.radioPublishAllCsd.Location = new System.Drawing.Point(15, 115);
            this.radioPublishAllCsd.Name = "radioPublishAllCsd";
            this.radioPublishAllCsd.Size = new System.Drawing.Size(113, 20);
            this.radioPublishAllCsd.TabIndex = 6;
            this.radioPublishAllCsd.TabStop = true;
            this.radioPublishAllCsd.Text = "发布全部 csd";
            this.radioPublishAllCsd.UseVisualStyleBackColor = true;
            // 
            // radioPublishPlist
            // 
            this.radioPublishPlist.AutoSize = true;
            this.radioPublishPlist.Location = new System.Drawing.Point(15, 145);
            this.radioPublishPlist.Name = "radioPublishPlist";
            this.radioPublishPlist.Size = new System.Drawing.Size(115, 20);
            this.radioPublishPlist.TabIndex = 7;
            this.radioPublishPlist.TabStop = true;
            this.radioPublishPlist.Text = "发布图集 plist";
            this.radioPublishPlist.UseVisualStyleBackColor = true;
            this.radioPublishPlist.CheckedChanged += new System.EventHandler(this.radioButton5_CheckedChanged);
            // 
            // textMergedTrans
            // 
            this.textMergedTrans.Location = new System.Drawing.Point(136, 198);
            this.textMergedTrans.Name = "textMergedTrans";
            this.textMergedTrans.Size = new System.Drawing.Size(525, 22);
            this.textMergedTrans.TabIndex = 8;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 204);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(112, 16);
            this.label2.TabIndex = 9;
            this.label2.Text = "新翻译文件路径";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 421);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(37, 16);
            this.label3.TabIndex = 10;
            this.label3.Text = "源码";
            // 
            // linkLabel1
            // 
            this.linkLabel1.AutoSize = true;
            this.linkLabel1.Location = new System.Drawing.Point(59, 421);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(195, 16);
            this.linkLabel1.TabIndex = 11;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "https://github.com/v012345/Trial";
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.linkLabel1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textMergedTrans);
            this.Controls.Add(this.radioPublishPlist);
            this.Controls.Add(this.radioPublishAllCsd);
            this.Controls.Add(this.radioPublishModifiedCsd);
            this.Controls.Add(this.radioExtract);
            this.Controls.Add(this.radioMergeTrans);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textCocosTool);
            this.Name = "Form1";
            this.Text = "多语言 ui 工具";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textCocosTool;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.RadioButton radioMergeTrans;
        private System.Windows.Forms.RadioButton radioExtract;
        private System.Windows.Forms.RadioButton radioPublishModifiedCsd;
        private System.Windows.Forms.RadioButton radioPublishAllCsd;
        private System.Windows.Forms.RadioButton radioPublishPlist;
        private System.Windows.Forms.TextBox textMergedTrans;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.LinkLabel linkLabel1;
    }
}

