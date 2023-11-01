#include "GameLib/Framework.h"
#include "GameLib/Gamelib.h"
using namespace GameLib;

#include "Image.h"
#include "State.h"

// 对象类
class State::Object {
  public:
    enum Type {
        OBJ_SPACE,
        OBJ_WALL,
        OBJ_BLOCK,
        OBJ_MAN,

        OBJ_UNKNOWN,
    };
    // 网格绘制函数
    enum ImageID {
        IMAGE_ID_PLAYER,
        IMAGE_ID_WALL,
        IMAGE_ID_BLOCK,
        IMAGE_ID_GOAL,
        IMAGE_ID_SPACE,
    };
    Object() : mType(OBJ_WALL), mGoalFlag(false), mMoveX(0), mMoveY(0) {}
    // 用舞台数据初始化自己
    void set(char c) {
        switch (c) {
            case '#': mType = OBJ_WALL; break;
            case ' ': mType = OBJ_SPACE; break;
            case 'o': mType = OBJ_BLOCK; break;
            case 'O':
                mType = OBJ_BLOCK;
                mGoalFlag = true;
                break;
            case '.':
                mType = OBJ_SPACE;
                mGoalFlag = true;
                break;
            case 'p': mType = OBJ_MAN; break;
            case 'P':
                mType = Object::OBJ_MAN;
                mGoalFlag = true;
                break;
        }
    }
    // 绘制背景
    void drawBackground(int x, int y, const Image* image) const {
        // 如果是墙，那就是墙
        if (mType == OBJ_WALL) {
            drawCell(x, y, IMAGE_ID_WALL, image);
        } else {
            if (mGoalFlag) {
                drawCell(x, y, IMAGE_ID_GOAL, image);
            } else {
                drawCell(x, y, IMAGE_ID_SPACE, image);
            }
        }
    }
    void drawForeground(int x, int y, const Image* image, int moveCount) const {
        // 只有人和行李移动。
        ImageID id = IMAGE_ID_SPACE; //
        if (mType == OBJ_BLOCK) {
            id = IMAGE_ID_BLOCK;
        } else if (mType == OBJ_MAN) {
            id = IMAGE_ID_PLAYER;
        }
        if (id != IMAGE_ID_SPACE) { // 背景以外
            const int m = State::MAX_MOVE_COUNT; // 太长了使用别名
            // 计算移动
            int dx = (mMoveX * (m - moveCount) * 32) / m;
            int dy = (mMoveY * (m - moveCount) * 32) / m;
            image->draw(x * 32 - dx, y * 32 - dy, id * 32, 0, 32, 32);
        }
    }
    static void drawCell(int x, int y, int id, const Image* image) { image->draw(x * 32, y * 32, id * 32, 0, 32, 32); }
    // 设置移动。第三个参数是替换的类型
    void move(int dx, int dy, Type replaced) {
        mMoveX = dx;
        mMoveY = dy;
        mType = replaced;
    }
    Type mType;
    bool mGoalFlag;
    int mMoveX;
    int mMoveY;
};

State::State(const char* stageData, int size) : mImage(0), mMoveCount(0), mStageData(0), mStageDataSize(size) {
    // 复制舞台数据以进行reset（）
    mStageData = new char[size + 1]; // 0末尾部分。
    for (int i = 0; i < size; ++i) { mStageData[i] = stageData[i]; }
    mStageData[size] = '\0'; // NULL终止
    // 初始化舞台
    reset();
    // 图片载入
    mImage = new Image(CMAKE_CURRENT_SOURCE_DIR "data/image/nimotsuKunImage2.dds");
}

State::~State() {
    SAFE_DELETE(mImage);
    SAFE_DELETE_ARRAY(mStageData); // 不要忘记
}

void State::reset() {
    // 尺寸测量
    setSize();
    // 数组分配
    mObjects.setSize(mWidth, mHeight);
    // 初始化舞台
    int x = 0;
    int y = 0;
    for (int i = 0; i < mStageDataSize; ++i) {
        Object t;
        switch (mStageData[i]) {
            case '#':
            case ' ':
            case 'o':
            case 'O':
            case '.':
            case 'p':
            case 'P':
                mObjects(x, y).set(mStageData[i]);
                ++x;
                break;
            case '\n':
                x = 0;
                ++y;
                break; // 换行处理
        }
    }
}

void State::setSize() {
    mWidth = mHeight = 0; // 初始化
    // 当前位置
    int x = 0;
    int y = 0;
    for (int i = 0; i < mStageDataSize; ++i) {
        switch (mStageData[i]) {
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
    // 分两步绘制。首先，绘制背景。
    for (int y = 0; y < mHeight; ++y) {
        for (int x = 0; x < mWidth; ++x) { mObjects(x, y).drawBackground(x, y, mImage); }
    }
    // 接下来绘制前景
    for (int y = 0; y < mHeight; ++y) {
        for (int x = 0; x < mWidth; ++x) { mObjects(x, y).drawForeground(x, y, mImage, mMoveCount); }
    }
}

void State::update(int dx, int dy) {
    // 当移动计数达到MAX_MOVE_COUNT
    if (mMoveCount >= MAX_MOVE_COUNT) {
        mMoveCount = 0; //
        // 初始化移动
        for (int y = 0; y < mHeight; ++y) {
            for (int x = 0; x < mWidth; ++x) {
                mObjects(x, y).mMoveX = 0;
                mObjects(x, y).mMoveY = 0;
            }
        }
    }
    // 移动时请勿更新。
    if (mMoveCount > 0) {
        ++mMoveCount;
        return;
    }
    // 使用简短的变量名。
    int w = mWidth;
    int h = mHeight;
    Array2D<Object>& o = mObjects;
    // 查找人坐标
    int x, y;
    x = y = -1;
    bool found = false;
    for (y = 0; y < mHeight; ++y) {
        for (x = 0; x < mWidth; ++x) {
            if (o(x, y).mType == Object::OBJ_MAN) {
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
    if (o(tx, ty).mType == Object::OBJ_SPACE) {
        o(tx, ty).move(dx, dy, Object::OBJ_MAN);
        o(x, y).move(dx, dy, Object::OBJ_SPACE);
        mMoveCount = 1; // 开始行动
        // B.该方向是箱子。如果该方向上的下一个格子为空白或目标，则移动。
    } else if (o(tx, ty).mType == Object::OBJ_BLOCK) {
        // 2检查方格是否在范围内
        int tx2 = tx + dx;
        int ty2 = ty + dy;
        if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) { // 不能按
            return;
        }
        if (o(tx2, ty2).mType == Object::OBJ_SPACE) {
            // 按顺序交换
            o(tx2, ty2).move(dx, dy, Object::OBJ_BLOCK);
            o(tx, ty).move(dx, dy, Object::OBJ_MAN);
            o(x, y).move(dx, dy, Object::OBJ_SPACE);
            mMoveCount = 1; // 开始行动
        }
    }
}

// 如果该块至少有一个goalFlag为假
// 尚未清除
bool State::hasCleared() const {
    for (int y = 0; y < mHeight; ++y) {
        for (int x = 0; x < mWidth; ++x) {
            if (mObjects(x, y).mType == Object::OBJ_BLOCK) {
                if (mObjects(x, y).mGoalFlag == false) { return false; }
            }
        }
    }
    return true;
}
