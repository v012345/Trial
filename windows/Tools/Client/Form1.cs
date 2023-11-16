using System.Diagnostics;
using System.IO;
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

        private void BtnNextStep_Click(object sender, EventArgs e)
        {
            // 发送消息给服务器
            string message = "Hello, Lua server!\n";
            byte[] data = Encoding.UTF8.GetBytes(message);
            mStream.Write(data, 0, data.Length);

            // 接收服务器的响应
            byte[] buffer = new byte[1024];
            int bytesRead = mStream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);
            Debug.WriteLine($"Re message to server: {response}");
        }
    }
}
