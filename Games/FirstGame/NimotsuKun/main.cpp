#include <iostream>
#include <stdlib.h>
using namespace std;

// #墙 _空白区 .终点 o砖块 p人
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

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

// 函数声明
void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);

int main() {
    //////////使用一维数组的原因可以参考本书相关章节
    Object* state = new Object[gStageWidth * gStageHeight]; // 分配状态数组空间

    initialize(state, gStageWidth, gStageHeight, gStageData); // 舞台初始化
    // 主循环
    while (true) {
        // 首先绘制
        draw(state, gStageWidth, gStageHeight);
        // 通关检测
        if (checkClear(state, gStageWidth, gStageHeight)) {
            break; // 通关检测
        }
        // 提示如何操作
        cout << "a:left s:right w:up z:down. command?" << endl; // 操作说明
        char input;
        cin >> input;
        // 更新
        update(state, input, gStageWidth, gStageHeight);
    }
    // 通关祝贺信息
    cout << "Congratulation's! you won." << endl;
    // 析构
    delete[] state;
    state = 0;

    // 为了避免运行完一闪而过，这里添加一个无限循环。命令行中按下Ctrl-C即可终止
    system("pause");
    return 0;
}

//---------------------下面是函数定义--------------

//////////以后可能会需要传入高度值，但是目前没用到，所以把参数变量名height先注释掉
void initialize(Object* state, int width, int /* height */, const char* stageData) {
    const char* d = stageData; // 数据读取位置
    int x = 0;
    int y = 0;
    while (*d != '\0') { // 不等于NULL
        Object t; // 临时变量，并没有特殊的含义而且被频繁使用一般可以命名为t。否则弄一个长命名读起来也费劲，没有必要
        switch (*d) {
            case '#': t = OBJ_WALL; break;
            case ' ': t = OBJ_SPACE; break;
            case 'o': t = OBJ_BLOCK; break;
            case 'O': t = OBJ_BLOCK_ON_GOAL; break;
            case '.': t = OBJ_GOAL; break;
            case 'p': t = OBJ_MAN; break;
            case 'P': t = OBJ_MAN_ON_GOAL; break;
            case '\n':
                x = 0;
                ++y;
                t = OBJ_UNKNOWN;
                break; // 换行处理
            default: t = OBJ_UNKNOWN; break;
        }
        ++d;
        if (t != OBJ_UNKNOWN) { // 这个if处理的意义在如果遇到未定义的元素值就跳过它
            state[y * width + x] = t; // 写入
            ++x;
        }
    }
}

void draw(const Object* state, int width, int height) {
    const char font[] = {' ', '#', '.', 'o', 'O', 'p', 'P'}; // Object的可能值
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Object o = state[y * width + x];
            cout << font[o];
        }
        cout << endl;
    }
}

// 第一个参数就是其他函数中的state，不过这里因为会频繁使用，
// 就缩写为s。另外两个参数w，h分别表示width,height
void update(Object* s, char input, int w, int h) {
    // 移动量变换(变量名中的d可以理解为difference或者delta)
    int dx = 0;
    int dy = 0;
    switch (input) {
        case 'a': dx = -1; break; // 向左
        case 's': dx = 1; break; // 右
        case 'w': dy = -1; break; // 上。Y朝下为正
        case 'z': dy = 1; break; // 下。
    }
    // 查询小人的坐标
    int i = -1;
    for (i = 0; i < w * h; ++i) {
        if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL) { break; }
    }
    int x = i % w; // x是对宽度的余数
    int y = i / w; // y是对宽度的商

    // 移动
    // 移动后的坐标（t并没有特定的代表含义）
    int tx = x + dx;
    int ty = y + dy;
    // 判断坐标的极端值。不允许超出合理值范围
    if (tx < 0 || ty < 0 || tx >= w || ty >= h) { return; }
    // A.该方向上是空白或者终点。小人则移动
    int p = y * w + x; // 人员位置
    int tp = ty * w + tx; // 目标位置（TargetPosition）
    if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL) {
        s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; // 如果该位置是终点，则将该位置值变为“终点上站着人”
        s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; // 如果该位置已经是“终点上站着人”，则变为“终点”
        // B.如果该方向上是箱子。并且该方向的下下个格子是空白或者终点，则允许移动
    } else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL) {
        // 检测同方向上的下下个格子是否位于合理值范围
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { // 按键无效
            return;
        }

        int tp2 = (ty + dy) * w + (tx + dx); // 下下个格子
        if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL) {
            // 按顺序替换
            s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
            s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
            s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
        }
    }
}

// 如果没有方块了则判定为通关
bool checkClear(const Object* s, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        if (s[i] == OBJ_BLOCK) { return false; }
    }
    return true;
}
