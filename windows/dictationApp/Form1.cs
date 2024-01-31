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
        private int mCount = 20;
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
                    this.PlaySound(key);
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
            myDictionary["あ"] = new double[] { 1.1, 1.8 };
            myDictionary["い"] = new double[] { 2.0, 2.6 };
            myDictionary["う"] = new double[] { 2.8, 3.5 };
            myDictionary["え"] = new double[] { 3.7, 4.5 };
            myDictionary["お"] = new double[] { 4.7, 5.4 };
            myDictionary["か"] = new double[] { 6.5, 7.3 };
            myDictionary["き"] = new double[] { 7.4, 8.2 };
            myDictionary["く"] = new double[] { 8.3, 9.0 };
            myDictionary["け"] = new double[] { 9.2, 10.0 };
            myDictionary["こ"] = new double[] { 10.1, 10.7 };
            myDictionary["さ"] = new double[] { 11.5, 12.6 };
            myDictionary["し"] = new double[] { 12.6, 13.5 };
            myDictionary["す"] = new double[] { 13.5, 14.3 };
            myDictionary["せ"] = new double[] { 14.3, 15.2 };
            myDictionary["そ"] = new double[] { 15.2, 16.0 };
            myDictionary["た"] = new double[] { 17.2, 18.0 };
            myDictionary["ち"] = new double[] { 18.0, 18.8 };
            myDictionary["つ"] = new double[] { 18.8, 19.8 };
            myDictionary["て"] = new double[] { 19.8, 20.6 };
            myDictionary["と"] = new double[] { 20.6, 21.4 };
            myDictionary["な"] = new double[] { 22.3, 23.3 };
            myDictionary["に"] = new double[] { 23.3, 24.1 };
            myDictionary["ぬ"] = new double[] { 24.2, 25.2 };
            myDictionary["ね"] = new double[] { 25.2, 25.9 };
            myDictionary["の"] = new double[] { 26.0, 26.7 };
            myDictionary["は"] = new double[] { 27.8, 28.7 };
            myDictionary["ひ"] = new double[] { 28.7, 29.5 };
            myDictionary["ふ"] = new double[] { 29.5, 30.3 };
            myDictionary["へ"] = new double[] { 30.5, 31.3 };
            myDictionary["ほ"] = new double[] { 31.3, 32.2 };
            myDictionary["ま"] = new double[] { 33.3, 34.1 };
            myDictionary["み"] = new double[] { 34.2, 35.0 };
            myDictionary["む"] = new double[] { 35.0, 35.9 };
            myDictionary["め"] = new double[] { 36.0, 36.8 };
            myDictionary["も"] = new double[] { 36.8, 37.7 };
            myDictionary["や"] = new double[] { 38.5, 39.6 };
            myDictionary["ゆ"] = new double[] { 39.6, 40.5 };
            myDictionary["よ"] = new double[] { 40.5, 41.4 };
            myDictionary["ら"] = new double[] { 42.4, 43.5 };
            myDictionary["り"] = new double[] { 43.5, 44.4 };
            myDictionary["る"] = new double[] { 44.4, 45.4 };
            myDictionary["れ"] = new double[] { 45.4, 46.3 };
            myDictionary["ろ"] = new double[] { 46.3, 47.2 };
            myDictionary["わ"] = new double[] { 48.1, 49.2 };
            myDictionary["を"] = new double[] { 49.3, 50.2 };
            myDictionary["ん"] = new double[] { 50.3, 51.2 };
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
                if (!this.isPlaying) {
                    this.PlaySound(this.mDictatingWord);
                }
            }
        }
        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {

        }
    }
}
