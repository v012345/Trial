#include "../Games/Framework.h"
using namespace GameLib;

#include "File.h"
#include "Image.h"
#include "State.h"

// 函数原型
void mainLoop();
void game(); // 游戏循环
void title(); // 主题画面循环

// 序列枚举
enum Sequence {
    SEQUENCE_GAME,
    SEQUENCE_TITLE,
};

// 全局变量
State* gState = 0; // 游戏状态
Image* gTitleImage = 0; // 主题画面
Sequence gSequence = SEQUENCE_TITLE; // 序列编号
int gCounter = 0;

// 用户封装函数。内容被抛出给mainLoop（）
namespace GameLib {
    void Framework::update() { mainLoop(); }
} // namespace GameLib

void mainLoop() {
    // 帧频调整
    Framework f = Framework::instance();
    f.setFrameRate(60); // 只需要调用一次

    if (gCounter % 60 == 0) { // 60每帧显示一次帧率
        cout << " FrameRate:" << f.frameRate() << endl;
    }
    ++gCounter;

    // 序列分支
    switch (gSequence) {
        case SEQUENCE_GAME: game(); break;
        case SEQUENCE_TITLE: title(); break;
    }
    // 结束判断（是否按下q或用鼠标按下X按钮）
    if (f.isKeyOn('q')) { f.requestEnd(); }
    if (f.isEndRequested()) {
        SAFE_DELETE(gState);
        SAFE_DELETE(gTitleImage);
    }
    f.sleep(10);
}

// 游戏循环。将大部分GameLoop（）移植到此处
void game() {
    // 初始化第一帧。
    if (!gState) {
        File file(CMAKE_CURRENT_SOURCE_DIR "stageData.txt");
        if (!(file.data())) { // 没有数据！
            cout << "stage file could not be read." << endl;
            return;
        }
        gState = new State(file.data(), file.size());
    }
    bool cleared = false;
    // 主循环
    // 清除检测
    if (gState->hasCleared()) { cleared = true; }
    // 获取输入
    Framework f = Framework::instance();
    int dx = 0;
    int dy = 0;
    if (f.isKeyOn('a')) {
        dx -= 1;
    } else if (f.isKeyOn('s')) {
        dx += 1;
    } else if (f.isKeyOn('w')) {
        dy -= 1;
    } else if (f.isKeyOn('z')) {
        dy += 1;
    }
    // 更新
    gState->update(dx, dy);
    // 绘制
    gState->draw();

    // 如果清除，请清理并转到标题画面
    if (cleared) {
        gSequence = SEQUENCE_TITLE;
        SAFE_DELETE(gState);
    }
}

void title() {
    // 初始化第一帧。
    if (!gTitleImage) { gTitleImage = new Image(CMAKE_CURRENT_SOURCE_DIR "title.dds"); }
    // 获取输入
    if (Framework::instance().isKeyOn(' ')) {
        gSequence = SEQUENCE_GAME;
        SAFE_DELETE(gTitleImage);
        return;
    }
    gTitleImage->draw(0, 0, 0, 0, gTitleImage->width(), gTitleImage->height());
}
