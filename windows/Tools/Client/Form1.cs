using System.Net.Sockets;
using System.Text;
namespace Client
{
    public partial class Form1 : Form
    {
        private string mServerIP = "127.0.0.1";
        private int mServerPort = 8849;
        private TcpClient mClient;
        private NetworkStream mStream;
        private bool mIsConnecting = false;
        public Form1()
        {
            InitializeComponent();
            Connect_To_Server();

        }
        private void Connect_To_Server()
        {
            try
            {
                // 创建TcpClient对象
                mClient = new TcpClient(mServerIP, mServerPort);
                mStream = mClient.GetStream();
                string message = "Hello from C# client!";
                byte[] data = Encoding.UTF8.GetBytes(message);
                mStream.Write(data, 0, data.Length);
                mIsConnecting = true;
                LblConnectionState.Text = "已连接";
                LblConnectionState.ForeColor = Color.Green;
            }
            catch (Exception ex)
            {
                mIsConnecting = false;
            }
        }
        private void BtnConnectServer_Click(object sender, EventArgs e)
        {
            if (!mIsConnecting)
            {
                Connect_To_Server();
            }
        }

        private void LblConnectionState_Click(object sender, EventArgs e)
        {

        }
    }
}
