#include "../Games/Framework.h"
using namespace GameLib;

#include "File.h"
#include "State.h"

// 函数原型
void mainLoop();

// 全局变量
State* gState = 0;
const int gFrameInterval = 16; // 16毫秒62.5fps。
unsigned gPreviousTime[10]; // 10记录框架时间
int gCounter = 0; // 一个计数器，用于计算主循环的次数

#define USE_SLEEP_FUNCTION // 使用sleep函数

// 用户封装函数。内容被抛出给mainLoop（）
namespace GameLib {
    void Framework::update() { mainLoop(); }
} // namespace GameLib

void mainLoop() {
    // 固定帧率处理。
    Framework f = Framework::instance();
    while ((f.time() - gPreviousTime[9]) < gFrameInterval) {
#ifdef USE_SLEEP_FUNCTION
        f.sleep(1);
#endif
    }
    // 帧率计算
    unsigned currentTime = f.time();
    unsigned frameInterval10 = currentTime - gPreviousTime[0];
    if (gCounter % 60 == 0) { // 60每帧显示一次帧率
        cout << "frameInterval:" << frameInterval10 / 10;
        cout << " FrameRate:" << 10 * 1000 / frameInterval10 << endl;
    }
    ++gCounter;
    // 更新历史
    for (int i = 0; i < 9; ++i) { gPreviousTime[i] = gPreviousTime[i + 1]; }
    gPreviousTime[9] = currentTime;

    // 初始化第一帧。
    if (!gState) {
        File file(CMAKE_CURRENT_SOURCE_DIR "stageData.txt");
        if (!(file.getData())) { // 没有数据！
            cout << "stage file could not be read." << endl;
            return;
        }
        gState = new State(file.getData(), file.getSize());
    }
    bool cleared = false;
    // 主循环
    // 清除检测
    if (gState->hasCleared()) { cleared = true; }
    // 获取输入
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

    if (cleared) {
        // 庆祝消息
        cout << "Congratulation! you win." << endl;
        delete gState;
        gState = 0;
    }
    // 结束判断
    if (f.isKeyOn('q')) { f.requestEnd(); }
    if (f.isEndRequested()) {
        if (gState) {
            delete gState;
            gState = 0;
        }
    }
}
