namespace NameToJson
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
            lbl_zhcn = new Label();
            lal_vi = new Label();
            lal_th = new Label();
            lbl_id = new Label();
            lbl_en = new Label();
            tbox_zhcn = new TextBox();
            tbox_vi = new TextBox();
            tbox_th = new TextBox();
            tbox_id = new TextBox();
            tbox_en = new TextBox();
            btn_gen = new Button();
            tbox_out = new TextBox();
            SuspendLayout();
            // 
            // lbl_zhcn
            // 
            lbl_zhcn.AutoSize = true;
            lbl_zhcn.Location = new Point(23, 38);
            lbl_zhcn.Name = "lbl_zhcn";
            lbl_zhcn.Size = new Size(50, 20);
            lbl_zhcn.TabIndex = 0;
            lbl_zhcn.Text = "zhcn : ";
            // 
            // lal_vi
            // 
            lal_vi.AutoSize = true;
            lal_vi.Location = new Point(42, 83);
            lal_vi.Name = "lal_vi";
            lal_vi.Size = new Size(31, 20);
            lal_vi.TabIndex = 1;
            lal_vi.Text = "vi : ";
            lal_vi.Click += lal_vi_Click;
            // 
            // lal_th
            // 
            lal_th.AutoSize = true;
            lal_th.Location = new Point(40, 127);
            lal_th.Name = "lal_th";
            lal_th.Size = new Size(33, 20);
            lal_th.TabIndex = 2;
            lal_th.Text = "th : ";
            lal_th.Click += lal_th_Click;
            // 
            // lbl_id
            // 
            lbl_id.AutoSize = true;
            lbl_id.Location = new Point(40, 171);
            lbl_id.Name = "lbl_id";
            lbl_id.Size = new Size(33, 20);
            lbl_id.TabIndex = 3;
            lbl_id.Text = "id : ";
            // 
            // lbl_en
            // 
            lbl_en.AutoSize = true;
            lbl_en.Location = new Point(37, 216);
            lbl_en.Name = "lbl_en";
            lbl_en.Size = new Size(36, 20);
            lbl_en.TabIndex = 4;
            lbl_en.Text = "en : ";
            // 
            // tbox_zhcn
            // 
            tbox_zhcn.Location = new Point(79, 35);
            tbox_zhcn.Name = "tbox_zhcn";
            tbox_zhcn.Size = new Size(454, 27);
            tbox_zhcn.TabIndex = 5;
            // 
            // tbox_vi
            // 
            tbox_vi.Location = new Point(79, 83);
            tbox_vi.Name = "tbox_vi";
            tbox_vi.Size = new Size(454, 27);
            tbox_vi.TabIndex = 6;
            // 
            // tbox_th
            // 
            tbox_th.Location = new Point(79, 124);
            tbox_th.Name = "tbox_th";
            tbox_th.Size = new Size(454, 27);
            tbox_th.TabIndex = 7;
            // 
            // tbox_id
            // 
            tbox_id.Location = new Point(79, 171);
            tbox_id.Name = "tbox_id";
            tbox_id.Size = new Size(454, 27);
            tbox_id.TabIndex = 8;
            // 
            // tbox_en
            // 
            tbox_en.Location = new Point(79, 213);
            tbox_en.Name = "tbox_en";
            tbox_en.Size = new Size(454, 27);
            tbox_en.TabIndex = 9;
            // 
            // btn_gen
            // 
            btn_gen.Location = new Point(594, 409);
            btn_gen.Name = "btn_gen";
            btn_gen.Size = new Size(194, 29);
            btn_gen.TabIndex = 10;
            btn_gen.Text = "生成 Json";
            btn_gen.UseVisualStyleBackColor = true;
            btn_gen.Click += btn_gen_Click;
            // 
            // tbox_out
            // 
            tbox_out.Location = new Point(79, 266);
            tbox_out.Multiline = true;
            tbox_out.Name = "tbox_out";
            tbox_out.Size = new Size(454, 172);
            tbox_out.TabIndex = 11;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(tbox_out);
            Controls.Add(btn_gen);
            Controls.Add(tbox_en);
            Controls.Add(tbox_id);
            Controls.Add(tbox_th);
            Controls.Add(tbox_vi);
            Controls.Add(tbox_zhcn);
            Controls.Add(lbl_en);
            Controls.Add(lbl_id);
            Controls.Add(lal_th);
            Controls.Add(lal_vi);
            Controls.Add(lbl_zhcn);
            Name = "Form1";
            Text = "Form1";
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private Label lbl_zhcn;
        private Label lal_vi;
        private Label lal_th;
        private Label lbl_id;
        private Label lbl_en;
        private TextBox tbox_zhcn;
        private TextBox tbox_vi;
        private TextBox tbox_th;
        private TextBox tbox_id;
        private TextBox tbox_en;
        private Button btn_gen;
        private TextBox tbox_out;
    }
}
