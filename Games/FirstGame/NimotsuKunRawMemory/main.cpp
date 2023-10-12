#include <iostream> //必须包含这个头文件
#include <stdlib.h>
using namespace std;
/*

这个示例中只会用到m这个变量。
我们希望能够通过这个例子来窥探编译器后面的一些工作原理。
编程过程中我们都能体会到自由定义变量的便利，
这里我们尝试来探究这种机制是怎么实现的。

[内存的使用方法]

0     : 用于存放函数的参数和返回值
1-17  : 这块区域供函数内任意使用。在函数结束后将销毁
18    : 8 //宽度
19    : 5 //海拔高度
20-59 : 8x5的状态数组
60-99 : 用字符串存储的舞台数据
*/

char m[100]; // 内存。只用到100个字节

enum Object {
    OBJ_SPACE,
    OBJ_WALL,
    OBJ_GOAL,
    OBJ_BLOCK,
    OBJ_BLOCK_ON_GOAL,
    OBJ_MAN,
    OBJ_MAN_ON_GOAL,

    OBJ_UNKNOWN,
};

// 函数原型
void initializeGlobalVariables();
void initialize();
void draw();
void update();
void checkClear();

int main() {
    // 全局变量初始化
    initializeGlobalVariables();

    initialize(); // 舞台初始化
    // 主循环
    while (true) {
        // 首先是绘制
        draw();
        // 通关检测，结果放入m[0]中
        checkClear();
        if (m[0] == 1) {
            break; // 通关检测
        }
        // 提示如何操作
        cout << "a:left s:right w:up z:down. command?" << endl; // 操作说明
        // 输入值存放到m[ 0 ]中
        cin >> m[0];
        // 刷新
        update();
    }
    // 打印通关祝贺的信息
    cout << "Congratulation! you win." << endl;

    // 为了避免运行完一闪而过，这里添加一个无限循环。命令行中按下Ctrl-C即可终止
    system("pause");
    return 0;
}

//---------------------下面是函数定义--------------

// 该函数对m[60]开始的字符串进行解析，构建出状态数组存放到m[20]开始的位置
void initialize() {
    m[0] = 0; // 0号位置表示应该读取的下标值
    m[1] = 0; // 1号位置表示当前的x坐标
    m[2] = 0; // 2号位置表示当前的y坐标

    while (m[60 + m[0]] != '\0') { // NULL之前的所有字符
        // 存入3号位置
        switch (m[60 + m[0]]) {
            case '#': m[3] = OBJ_WALL; break;
            case ' ': m[3] = OBJ_SPACE; break;
            case 'o': m[3] = OBJ_BLOCK; break;
            case 'O': m[3] = OBJ_BLOCK_ON_GOAL; break;
            case '.': m[3] = OBJ_GOAL; break;
            case 'p': m[3] = OBJ_MAN; break;
            case 'P': m[3] = OBJ_MAN_ON_GOAL; break;
            case '\n':
                m[1] = 0;
                ++m[2];
                m[3] = OBJ_UNKNOWN;
                break; // 换行处理(++y)
            default: m[3] = OBJ_UNKNOWN; break;
        }
        ++m[0];
        if (m[3] != OBJ_UNKNOWN) { // if判断用于忽略那些无法识别的字符
            m[20 + m[2] * m[18] + m[1]] = m[3]; // 存起来 m[18]表示宽度
            ++m[1]; //++x
        }
    }
}

void draw() {
    for (m[0] = 0; m[0] < m[19]; ++m[0]) {
        for (m[1] = 0; m[1] < m[18]; ++m[1]) {
            m[2] = m[20 + m[0] * m[18] + m[1]];
            switch (m[2]) {
                case OBJ_SPACE: cout << ' '; break;
                case OBJ_WALL: cout << '#'; break;
                case OBJ_GOAL: cout << '.'; break;
                case OBJ_BLOCK: cout << 'o'; break;
                case OBJ_BLOCK_ON_GOAL: cout << 'O'; break;
                case OBJ_MAN: cout << 'p'; break;
                case OBJ_MAN_ON_GOAL: cout << 'P'; break;
            }
        }
        cout << endl;
    }
}

void update() {
    // 移动量变换
    m[1] = 0; // dx
    m[2] = 0; // dy
    switch (m[0]) { // 把input存入m[100]
        case 'a': m[1] = -1; break; // 向左
        case 's': m[1] = 1; break; // 右
        case 'w': m[2] = -1; break; // 上。Y朝下为正
        case 'z': m[2] = 1; break; // 下。
    }
    // 查找小人的坐标
    for (m[0] = 0; m[0] < m[18] * m[19]; ++m[0]) {
        if (m[20 + m[0]] == OBJ_MAN || m[20 + m[0]] == OBJ_MAN_ON_GOAL) { break; }
    }
    m[3] = m[0] % m[18]; // x是对宽度的余数
    m[4] = m[0] / m[18]; // y是对宽度的商

    // 移动
    // 移动后的坐标
    m[5] = m[3] + m[1]; // tx
    m[6] = m[4] + m[2]; // ty
    // 判断坐标的极端值。不允许超出合理值范围
    if (m[5] < 0 || m[6] < 0 || m[5] >= m[18] || m[6] >= m[19]) { return; }
    // A.该方向上是空白或者终点。小人则移动
    m[7] = 20 + m[4] * m[18] + m[3]; // 小人的位置
    m[8] = 20 + m[6] * m[18] + m[5]; // 欲前往目标的位置
    if (m[m[8]] == OBJ_SPACE || m[m[8]] == OBJ_GOAL) { // 如果要前往的位置还有空间
        m[m[8]] = (m[m[8]] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; // 如果该位置是终点，则将该位置值变为“终点上站着人”
        m[m[7]] = (m[m[7]] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; // 如果该位置已经是“终点上站着人”，则变为“终点”
        // B.如果该方向上是箱子。并且该方向的下下个格子是空白或者终点，则允许移动
    } else if (m[m[8]] == OBJ_BLOCK || m[m[8]] == OBJ_BLOCK_ON_GOAL) {
        // 检测同方向上的下下个格子是否位于合理值范围
        m[9] = m[5] + m[1];
        m[10] = m[6] + m[2];
        if (m[9] < 0 || m[10] < 0 || m[9] >= m[18] || m[10] >= m[19]) { // 按键输入无效
            return;
        }

        m[11] = 20 + (m[6] + m[2]) * m[18] + (m[5] + m[1]); // 下下个格子
        if (m[m[11]] == OBJ_SPACE || m[m[11]] == OBJ_GOAL) {
            // 按顺序替换
            m[m[11]] = (m[m[11]] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
            m[m[8]] = (m[m[8]] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
            m[m[7]] = (m[m[7]] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
        }
    }
}

// 如果没有区块了则视为通关
void checkClear() {
    for (m[1] = 20; m[1] < 20 + m[18] * m[19]; ++m[1]) {
        if (m[m[1]] == OBJ_BLOCK) {
            m[0] = 0; // 返回值放入m[ 0 ]
            return;
        }
    }
    m[0] = 1; // 返回值放入m[ 0 ]
    return;
}

// #墙 _空白区 .终点 o砖块 p人
// ########
// # .. p #
// # oo   #
// #      #
// ########";
void initializeGlobalVariables() {
    // 宽度为18
    m[18] = 8;
    // 高度为19
    m[19] = 5;
    // 第2行
    m[68] = '#';
    m[69] = ' ';
    m[70] = '.';
    m[71] = '.';
    m[72] = ' ';
    m[73] = 'p';
    m[74] = ' ';
    m[75] = '#';
    // 第3行
    m[76] = '#';
    m[77] = ' ';
    m[78] = 'o';
    m[79] = 'o';
    m[80] = ' ';
    m[81] = ' ';
    m[82] = ' ';
    m[83] = '#';
    // 第4行
    m[84] = '#';
    m[85] = ' ';
    m[86] = ' ';
    m[87] = ' ';
    m[88] = ' ';
    m[89] = ' ';
    m[90] = ' ';
    m[91] = '#';
    // 第1行和第5行全部#
    for (m[0] = 0; m[0] < m[18]; ++m[0]) {
        m[60 + m[0]] = '#';
        m[92 + m[0]] = '#';
    }
}
