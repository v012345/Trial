using System;
using NAudio.Wave;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
namespace dictationApp
{

    public partial class Form1 : Form
    {
        private Mp3FileReader reader;
        private bool isDicating = false;
        private int mCount = 20;
        List<string> selectedKeys = new List<string>();
        List<string> dicatedKeys = new List<string>();
        // 创建一个Dictionary，其中key是string类型，value是一个数组（double[]）
        private Dictionary<string, double[]> myDictionary = new Dictionary<string, double[]>();
       
        public Form1()
        {
            InitializeComponent();
            // 获取当前程序集的引用
            Assembly assembly = Assembly.GetExecutingAssembly();
            // 构建嵌入资源的完整名称
            string resourceName = $"{assembly.GetName().Name}.01.mp3";
            Stream resourceStream = assembly.GetManifestResourceStream(resourceName);
            reader = new Mp3FileReader(resourceStream);
            // 在构造函数中设置窗口边框样式
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            this.MaximizeBox = false; // 禁用最大化按钮
                                      //  this.MinimizeBox = false; // 禁用最小化按钮
        }

        private void BtnPlay_Click(object sender, EventArgs e)
        {
            if (this.isDicating)
            {
                if (this.selectedKeys.Count <= 0)
                {
                    this.isDicating = false;
                    this.BtnPlay.Text = "再次开始";
                }
                else
                {
                    string key = this.selectedKeys[0];
                    this.selectedKeys.Remove(key);
                    this.dicatedKeys.Add(key);
                    double[] s = this.myDictionary[key];
                    TimeSpan startTime = TimeSpan.FromSeconds(s[0]);  // 设置开始时间
                    TimeSpan endTime = TimeSpan.FromSeconds(s[1]);    // 设置结束时间
                                                                      // 这个不优化了, 一直初始化也没有什么影响
                   
                        WaveOutEvent waveOut = new WaveOutEvent();
                        waveOut.Init(reader);

                        // 跳到指定的开始时间
                        reader.CurrentTime = startTime;

                        this.BtnPlay.Enabled = false;
                        waveOut.Play();

                        // 播放至指定的结束时间
                        while (waveOut.PlaybackState == PlaybackState.Playing && reader.CurrentTime < endTime)
                        {
                            System.Threading.Thread.Sleep(1);
                        }

                        waveOut.Stop();
                        this.BtnPlay.Enabled = true;
                    
                }

            }
            else
            {
                this.isDicating = true;
                this.selectedKeys = this.GetRandomKeys(this.myDictionary, this.mCount);
                this.dicatedKeys.Clear();
                this.BtnPlay.Text = "播放";
            }
            List<string> words = new List<string>();
            
            for (int i = 0; i < this.dicatedKeys.Count; i++)
            {
                words.Add(Convert.ToString(i + 1));
                words.Add(" ");
                if (this.selectedKeys.Count <= 0) {
                    words.Add(this.dicatedKeys[i]);
                }
                words.Add(Environment.NewLine);
            }
            this.TBoxHiragana.Text = string.Join("", words);

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // 添加数据
            myDictionary["あ"] = new double[] { 1.1, 2.1 };
            myDictionary["い"] = new double[] { 3.5, 4.5 };
            myDictionary["う"] = new double[] { 4.1, 6.1 };
            myDictionary["え"] = new double[] { 7.5, 8.5 };
            myDictionary["お"] = new double[] { 7.5, 8.5 };
            myDictionary["か"] = new double[] { 7.5, 8.5 };
            myDictionary["き"] = new double[] { 7.5, 8.5 };
            myDictionary["く"] = new double[] { 7.5, 8.5 };
            myDictionary["け"] = new double[] { 7.5, 8.5 };
            myDictionary["こ"] = new double[] { 7.5, 8.5 };
            myDictionary["さ"] = new double[] { 7.5, 8.5 };
            myDictionary["し"] = new double[] { 7.5, 8.5 };
            myDictionary["す"] = new double[] { 7.5, 8.5 };
            myDictionary["せ"] = new double[] { 7.5, 8.5 };
            myDictionary["そ"] = new double[] { 7.5, 8.5 };
            myDictionary["た"] = new double[] { 7.5, 8.5 };
            myDictionary["ち"] = new double[] { 7.5, 8.5 };
            myDictionary["つ"] = new double[] { 7.5, 8.5 };
            myDictionary["て"] = new double[] { 7.5, 8.5 };
            myDictionary["と"] = new double[] { 7.5, 8.5 };
            myDictionary["な"] = new double[] { 7.5, 8.5 };
            myDictionary["に"] = new double[] { 7.5, 8.5 };
            myDictionary["ぬ"] = new double[] { 7.5, 8.5 };
            myDictionary["ね"] = new double[] { 7.5, 8.5 };
            myDictionary["の"] = new double[] { 7.5, 8.5 };
            myDictionary["は"] = new double[] { 7.5, 8.5 };
            myDictionary["ひ"] = new double[] { 7.5, 8.5 };
            myDictionary["ふ"] = new double[] { 7.5, 8.5 };
            myDictionary["へ"] = new double[] { 7.5, 8.5 };
            myDictionary["ほ"] = new double[] { 7.5, 8.5 };
            myDictionary["ま"] = new double[] { 7.5, 8.5 };
            myDictionary["み"] = new double[] { 7.5, 8.5 };
            myDictionary["む"] = new double[] { 7.5, 8.5 };
            myDictionary["め"] = new double[] { 7.5, 8.5 };
            myDictionary["も"] = new double[] { 7.5, 8.5 };
            myDictionary["や"] = new double[] { 7.5, 8.5 };
            myDictionary["ゆ"] = new double[] { 7.5, 8.5 };
            myDictionary["よ"] = new double[] { 7.5, 8.5 };
            myDictionary["ら"] = new double[] { 7.5, 8.5 };
            myDictionary["り"] = new double[] { 7.5, 8.5 };
            myDictionary["る"] = new double[] { 7.5, 8.5 };
            myDictionary["れ"] = new double[] { 7.5, 8.5 };
            myDictionary["ろ"] = new double[] { 7.5, 8.5 };
            myDictionary["わ"] = new double[] { 7.5, 8.5 };
            myDictionary["を"] = new double[] { 7.5, 8.5 };
            myDictionary["ん"] = new double[] { 7.5, 8.5 };
            this.BtnPlay.Text = "开始";
            // 访问和打印数据
            //Debug.WriteLine("第一个: [{0}]", string.Join(", ", myDictionary["第一个"]));
            //Debug.WriteLine("第二个: [{0}]", string.Join(", ", myDictionary["第二个"]));
        }
        private List<string> GetRandomKeys(Dictionary<string, double[]> dictionary, int count)
        {
            Random random = new Random();
            List<string> keys = dictionary.Keys.ToList();

            // 随机打乱键的顺序
            keys = keys.OrderBy(k => random.Next()).ToList();

            // 选取指定数量的键
            List<string> selectedKeys = keys.Take(count).ToList();

            return selectedKeys;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {

        }

        private void BtnPlayAgain_Click(object sender, EventArgs e)
        {
            if (this.selectedKeys.Count>0)
            {
                string key = this.selectedKeys[0];
                double[] s = this.myDictionary[key];
                TimeSpan startTime = TimeSpan.FromSeconds(s[0]);  // 设置开始时间
                TimeSpan endTime = TimeSpan.FromSeconds(s[1]);    // 设置结束时间
                                                                  // 这个不优化了, 一直初始化也没有什么影响

                WaveOutEvent waveOut = new WaveOutEvent();
                waveOut.Init(reader);

                // 跳到指定的开始时间
                reader.CurrentTime = startTime;

                waveOut.Play();

                // 播放至指定的结束时间
                while (waveOut.PlaybackState == PlaybackState.Playing && reader.CurrentTime < endTime)
                {
                    System.Threading.Thread.Sleep(1);
                }

                waveOut.Stop();
            }
            
        }
    }
}
