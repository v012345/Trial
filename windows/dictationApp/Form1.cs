using NAudio.Wave;
using System.Reflection;
using System.Windows.Forms;
namespace dictationApp
{

    public partial class Form1 : Form
    {
        private Mp3FileReader reader;
        private bool isDicating = false;
        private bool isPlaying = false;
        private int mCount = 21;
        List<string> selectedKeys = new List<string>();
        List<string> dicatedKeys = new List<string>();
        private string mDictatingWord = null;
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
            if (this.isPlaying)
            {
                return;
            }
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
                    this.mDictatingWord = key;
                    if (this.selectedKeys.Count > 0)
                    {
                        this.PlaySound(key);
                    }
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
                if (this.selectedKeys.Count <= 0)
                {
                    words.Add(this.dicatedKeys[i]);
                }
                words.Add(Environment.NewLine);

            }
            this.TBoxHiragana.Text = string.Join("", words);

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // 添加数据
            //myDictionary["あ"] = new double[] { 1.1, 1.8 };
            //mydictionary["い"] = new double[] { 2.0, 2.6 };
            //mydictionary["う"] = new double[] { 2.8, 3.5 };
            //mydictionary["え"] = new double[] { 3.7, 4.5 };
            //mydictionary["お"] = new double[] { 4.7, 5.4 };
            //mydictionary["か"] = new double[] { 6.5, 7.3 };
            //mydictionary["き"] = new double[] { 7.4, 8.2 };
            //mydictionary["く"] = new double[] { 8.3, 9.0 };
            //mydictionary["け"] = new double[] { 9.2, 10.0 };
            //mydictionary["こ"] = new double[] { 10.1, 10.7 };
            //mydictionary["さ"] = new double[] { 11.5, 12.6 };
            //mydictionary["し"] = new double[] { 12.6, 13.5 };
            //mydictionary["す"] = new double[] { 13.5, 14.3 };
            //mydictionary["せ"] = new double[] { 14.3, 15.2 };
            //mydictionary["そ"] = new double[] { 15.2, 16.0 };
            //mydictionary["た"] = new double[] { 17.2, 18.0 };
            //mydictionary["ち"] = new double[] { 18.0, 18.8 };
            //mydictionary["つ"] = new double[] { 18.8, 19.8 };
            //mydictionary["て"] = new double[] { 19.8, 20.6 };
            //mydictionary["と"] = new double[] { 20.6, 21.4 };
            //mydictionary["な"] = new double[] { 22.3, 23.3 };
            //mydictionary["に"] = new double[] { 23.3, 24.1 };
            //mydictionary["ぬ"] = new double[] { 24.2, 25.2 };
            //mydictionary["ね"] = new double[] { 25.2, 25.9 };
            //mydictionary["の"] = new double[] { 26.0, 26.7 };
            //mydictionary["は"] = new double[] { 27.8, 28.7 };
            //mydictionary["ひ"] = new double[] { 28.7, 29.5 };
            //mydictionary["ふ"] = new double[] { 29.5, 30.3 };
            //mydictionary["へ"] = new double[] { 30.5, 31.3 };
            //mydictionary["ほ"] = new double[] { 31.3, 32.2 };
            //mydictionary["ま"] = new double[] { 33.3, 34.1 };
            //mydictionary["み"] = new double[] { 34.2, 35.0 };
            //mydictionary["む"] = new double[] { 35.0, 35.9 };
            //mydictionary["め"] = new double[] { 36.0, 36.8 };
            //mydictionary["も"] = new double[] { 36.8, 37.7 };
            //mydictionary["や"] = new double[] { 38.5, 39.6 };
            //mydictionary["ゆ"] = new double[] { 39.6, 40.5 };
            //mydictionary["よ"] = new double[] { 40.5, 41.4 };
            //mydictionary["ら"] = new double[] { 42.4, 43.5 };
            //mydictionary["り"] = new double[] { 43.5, 44.4 };
            //mydictionary["る"] = new double[] { 44.4, 45.4 };
            //mydictionary["れ"] = new double[] { 45.4, 46.3 };
            //mydictionary["ろ"] = new double[] { 46.3, 47.2 };
            //mydictionary["わ"] = new double[] { 48.1, 49.2 };
            //mydictionary["を"] = new double[] { 49.3, 50.2 };
            //mydictionary["ん"] = new double[] { 50.3, 51.2 };

            //mydictionary["が"] = new double[] { 53.3, 54.3 };
            //mydictionary["ぎ"] = new double[] { 54.3, 55.2 };
            //mydictionary["ぐ"] = new double[] { 55.2, 56.0 };
            //mydictionary["げ"] = new double[] { 56.0, 56.8 };
            //mydictionary["ご"] = new double[] { 57.0, 57.8 };

            //mydictionary["ざ"] = new double[] { 59.0, 60.0 };
            //mydictionary["じ"] = new double[] { 60.0, 60.8 };
            //mydictionary["ず"] = new double[] { 61.0, 61.8 };
            //mydictionary["ぜ"] = new double[] { 61.8, 62.8 };
            //mydictionary["ぞ"] = new double[] { 62.8, 63.8 };

            //mydictionary["だ"] = new double[] { 65.5, 66.2 };
            //mydictionary["ぢ"] = new double[] { 66.3, 67.2 };
            //mydictionary["づ"] = new double[] { 67.2, 68.0 };
            //mydictionary["で"] = new double[] { 68.1, 68.9 };
            //mydictionary["ど"] = new double[] { 69.0, 69.9 };

            //mydictionary["ば"] = new double[] { 71.4, 72.1 };
            //mydictionary["び"] = new double[] { 72.2, 73.0 };
            //mydictionary["ぶ"] = new double[] { 73.0, 73.8 };
            //mydictionary["べ"] = new double[] { 74.0, 74.8 };
            //mydictionary["ぼ"] = new double[] { 74.8, 75.6 };

            //mydictionary["ぱ"] = new double[] { 77.0, 77.8 };
            //mydictionary["ぴ"] = new double[] { 77.9, 78.8 };
            //mydictionary["ぷ"] = new double[] { 78.8, 79.5 };
            //mydictionary["ぺ"] = new double[] { 79.6, 80.3 };
            //mydictionary["ぽ"] = new double[] { 80.4, 81.2 };


            
            myDictionary["きゃ"] = new double[] { 82.8, 83.4 };
            myDictionary["きゅ"] = new double[] { 83.8, 84.5 };
            myDictionary["きょ"] = new double[] { 84.6, 85.4 };

            myDictionary["ぎゃ"] = new double[] { 107.3, 108.2 };
            myDictionary["ぎゅ"] = new double[] { 108.2, 109.0 };
            myDictionary["ぎょ"] = new double[] { 109.0, 110.0 };

            myDictionary["しゃ"] = new double[] { 86.2, 87.1 };
            myDictionary["しゅ"] = new double[] { 87.1, 88.0 };
            myDictionary["しょ"] = new double[] { 88.0, 88.9 };


            myDictionary["じゃ"] = new double[] { 110.9, 111.8 };
            myDictionary["じゅ"] = new double[] { 111.8, 112.7 };
            myDictionary["じょ"] = new double[] { 112.7, 113.7 };


            myDictionary["ちゃ"] = new double[] { 90.0, 90.8 };
            myDictionary["ちゅ"] = new double[] { 90.8, 91.7 };
            myDictionary["ちょ"] = new double[] { 91.7, 92.7 };

            myDictionary["にゃ"] = new double[] { 93.5, 94.5 };
            myDictionary["にゅ"] = new double[] { 94.5, 95.4 };
            myDictionary["にょ"] = new double[] { 95.4, 96.3 };

            myDictionary["ひゃ"] = new double[] { 97.0, 98.0 };
            myDictionary["ひゅ"] = new double[] { 98.0, 98.8 };
            myDictionary["ひょ"] = new double[] { 98.8, 99.7 };

            myDictionary["みゃ"] = new double[] { 100.4, 101.3 };
            myDictionary["みゅ"] = new double[] { 101.3, 102.1 };
            myDictionary["みょ"] = new double[] { 102.2, 103.0 };

            myDictionary["りゃ"] = new double[] { 104.0, 104.7 };
            myDictionary["りゅ"] = new double[] { 104.7, 105.5 };
            myDictionary["りょ"] = new double[] { 105.5, 106.3 };



            myDictionary["びゃ"] = new double[] { 114.7, 115.4 };
            myDictionary["びゅ"] = new double[] { 115.4, 116.3 };
            myDictionary["びょ"] = new double[] { 116.3, 117.3 };

            myDictionary["ぴゃ"] = new double[] { 118.3, 119.1 };
            myDictionary["ぴゅ"] = new double[] { 119.1, 120.0 };
            myDictionary["ぴょ"] = new double[] { 120.0, 120.9 };

            

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

        private async void PlaySound(string world)
        {
            this.isPlaying = true;
            double[] s = this.myDictionary[world];

            TimeSpan startTime = TimeSpan.FromSeconds(s[0]);  // 设置开始时间
            TimeSpan endTime = TimeSpan.FromSeconds(s[1]);    // 设置结束时间

            WaveOutEvent waveOut = new WaveOutEvent();
            waveOut.Init(this.reader);

            // 跳到指定的开始时间
            this.reader.CurrentTime = startTime;

            this.BtnPlay.Enabled = false;
           // this.BtnPlayAgain.Enabled = false;
            waveOut.Play();

            // 异步等待播放完成或达到指定的结束时间
            await Task.Delay(endTime - startTime);

            waveOut.Stop();
            this.BtnPlay.Enabled = true;
            // this.BtnPlayAgain.Enabled = true;
            this.isPlaying = false;
        }

        private void BtnPlayAgain_Click(object sender, EventArgs e)
        {
            if (this.mDictatingWord != null)
            {
                if (!this.isPlaying & this.selectedKeys.Count > 0) {
                    this.PlaySound(this.mDictatingWord);
                }
            }
        }
        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {

        }
    }
}
