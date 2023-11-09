using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection.Emit;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Button;

namespace LangManager
{
    public partial class Form1 : Form
    {
        private bool isWorking = false;
        public Form1()
        {
            InitializeComponent();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (isWorking)
            {
                return;
            }
            string cmdline = String.Format("\"{0}\" ", textCocosTool.Text);
            if (radioMergeTrans.Checked)
            {
                cmdline +=String.Format("\"{0}\" ", "merge");
                
            }
            if (radioExtract.Checked)
            {
                cmdline += String.Format("\"{0}\" ", "extract");

            }
            if (radioPublishModifiedCsd.Checked)
            {
                cmdline += String.Format("\"{0}\" ", "publish");

            }
            if (radioPublishAllCsd.Checked)
            {
                cmdline += String.Format("\"{0}\" ", "publishAll");

            }
            if (radioPublishPlist.Checked)
            {
                cmdline += String.Format("\"{0}\" ", "plist");
            }
            cmdline += String.Format("\"{0}\" ", textMergedTrans.Text);
            isWorking = true;
            Task.Factory.StartNew(() =>
            {
                var process = System.Diagnostics.Process.Start("lua.exe", cmdline);
                process.WaitForExit();
            }).ContinueWith((t) =>
            {
                isWorking = false;
            });
        }

        private void radioButton5_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/v012345/Trial");
        }
    }
}
