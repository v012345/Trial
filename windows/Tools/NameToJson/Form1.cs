using System;
using System.Diagnostics;
using System.Text.Encodings.Web;
using System.Text.Json;

namespace NameToJson
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void lal_th_Click(object sender, EventArgs e)
        {

        }

        private void lal_vi_Click(object sender, EventArgs e)
        {

        }

        private void btn_gen_Click(object sender, EventArgs e)
        {
            Lang lang = new Lang
            {
                zhcn = tbox_zhcn.Text,
                vi = tbox_vi.Text,
                th = tbox_th.Text,
                id = tbox_id.Text,
                en = tbox_en.Text
            };
            JsonSerializerOptions options = new JsonSerializerOptions
            {
                Encoder = JavaScriptEncoder.UnsafeRelaxedJsonEscaping // 关闭 Unicode 转义
            };
            Debug.WriteLine(JsonSerializer.Serialize(lang, options));
            tbox_out.Text = JsonSerializer.Serialize(lang, options);

        }
    }
    public class Lang
    {
        public string zhcn { get; set; }
        public string vi { get; set; }
        public string th { get; set; }
        public string id { get; set; }
        public string en { get; set; }
    }
}
