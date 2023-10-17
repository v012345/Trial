#include "State.h"
#include "Image.h"

State::State(const char* stageData, int size) : mImage(0) {
    // 尺寸测量
    setSize(stageData, size);
    // 数组分配
    mObjects.setSize(mWidth, mHeight);
    mGoalFlags.setSize(mWidth, mHeight);
    // 填写初始值
    for (int y = 0; y < mHeight; ++y) {
        for (int x = 0; x < mWidth; ++x) {
            mObjects(x, y) = OBJ_WALL; // 多余的部分是墙
            mGoalFlags(x, y) = false; // 不是目标
        }
    }
    int x = 0;
    int y = 0;
    for (int i = 0; i < size; ++i) {
        Object t;
        bool goalFlag = false;
        switch (stageData[i]) {
            case '#': t = OBJ_WALL; break;
            case ' ': t = OBJ_SPACE; break;
            case 'o': t = OBJ_BLOCK; break;
            case 'O':
                t = OBJ_BLOCK;
                goalFlag = true;
                break;
            case '.':
                t = OBJ_SPACE;
                goalFlag = true;
                break;
            case 'p': t = OBJ_MAN; break;
            case 'P':
                t = OBJ_MAN;
                goalFlag = true;
                break;
            case '\n':
                x = 0;
                ++y;
                t = OBJ_UNKNOWN;
                break; // 换行处理
            default: t = OBJ_UNKNOWN; break;
        }
        if (t != OBJ_UNKNOWN) { // 这个if判断会忽略无法识别的字符
            mObjects(x, y) = t; // 写入
            mGoalFlags(x, y) = goalFlag; // 目标信息
            ++x;
        }
    }
    // 图片载入
    mImage = new Image(CMAKE_CURRENT_SOURCE_DIR "nimotsuKunImage2.dds");
}

State::~State() {
    delete mImage;
    mImage = 0;
}

void State::setSize(const char* stageData, int size) {
    const char* d = stageData; // 读取指针
    mWidth = mHeight = 0; // 初始化
    // 当前位置
    int x = 0;
    int y = 0;
    for (int i = 0; i < size; ++i) {
        switch (stageData[i]) {
            case '#':
            case ' ':
            case 'o':
            case 'O':
            case '.':
            case 'p':
            case 'P': ++x; break;
            case '\n':
                ++y;
                // 最大值更新
                mWidth = (mWidth > x) ? mWidth : x;
                mHeight = (mHeight > y) ? mHeight : y;
                x = 0;
                break;
        }
    }
}

void State::draw() const {
    for (int y = 0; y < mHeight; ++y) {
        for (int x = 0; x < mWidth; ++x) {
            Object o = mObjects(x, y);
            bool goalFlag = mGoalFlags(x, y);
            // 如果不是墙，绘制地板
            if (o != OBJ_WALL) {
                if (goalFlag) {
                    drawCell(x, y, IMAGE_ID_GOAL);
                } else {
                    drawCell(x, y, IMAGE_ID_SPACE);
                }
            }
            ImageID id = IMAGE_ID_SPACE;
            switch (o) {
                case OBJ_WALL: id = IMAGE_ID_WALL; break;
                case OBJ_BLOCK: id = IMAGE_ID_BLOCK; break;
                case OBJ_MAN: id = IMAGE_ID_PLAYER; break;
            }
            if (id != IMAGE_ID_SPACE) { // 地板已经绘制了，所以没有必要
                drawCell(x, y, id);
            }
        }
    }
}

void State::drawCell(int x, int y, ImageID id) const { mImage->draw(x * 32, y * 32, id * 32, 0, 32, 32); }

void State::update(char input) {
    // 移动差分变换
    int dx = 0;
    int dy = 0;
    switch (input) {
        case 'a': dx = -1; break; // 向左
        case 's': dx = 1; break; // 右
        case 'w': dy = -1; break; // 上。Y朝下为正
        case 'z': dy = 1; break; // 下。
    }
    // 使用简短的变量名。
    int w = mWidth;
    int h = mHeight;
    Array2D<Object>& o = mObjects;
    // 查找人坐标
    int x, y;
    bool found = false;
    for (y = 0; y < mHeight; ++y) {
        for (x = 0; x < mWidth; ++x) {
            if (o(x, y) == OBJ_MAN) {
                found = true;
                break;
            }
        }
        if (found) { break; }
    }
    // 移动
    // 运动后坐标
    int tx = x + dx;
    int ty = y + dy;
    // 检查最大和最小坐标。
    if (tx < 0 || ty < 0 || tx >= w || ty >= h) { return; }
    // A.该方向上是空白或者终点。则小人移动
    if (o(tx, ty) == OBJ_SPACE) {
        o(tx, ty) = OBJ_MAN;
        o(x, y) = OBJ_SPACE;
        // B.该方向是箱子。如果该方向上的下一个格子为空白或目标，则移动。
    } else if (o(tx, ty) == OBJ_BLOCK) {
        // 2检查方格是否在范围内
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { // 不能按
            return;
        }
        if (o(tx2, ty2) == OBJ_SPACE) {
            // 按顺序交换
            o(tx2, ty2) = OBJ_BLOCK;
            o(tx, ty) = OBJ_MAN;
            o(x, y) = OBJ_SPACE;
        }
    }
}

// 如果该块至少有一个goalFlag为假
// 尚未清除
bool State::hasCleared() const {
    for (int y = 0; y < mHeight; ++y) {
        for (int x = 0; x < mWidth; ++x) {
            if (mObjects(x, y) == OBJ_BLOCK) {
                if (mGoalFlags(x, y) == false) { return false; }
            }
        }
    }
    return true;
}
