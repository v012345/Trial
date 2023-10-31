#include "GameLib/Framework.h"
using namespace GameLib;

#include "File.h"
#include "State.h"

// 函数原型
void mainLoop();

// 全局变量
State* gState = 0;

// 用户封装函数。内容被抛出给mainLoop（）
namespace GameLib {
    void Framework::update() { mainLoop(); }
} // namespace GameLib

void mainLoop() {
    // ×按钮被按下了吗？
    if (Framework::instance().isEndRequested()) {
        if (gState) {
            delete gState;
            gState = 0;
        }
        return;
    }
    // 初始化第一帧。绘制第一个状态并完成。
    if (!gState) {
        File file(CMAKE_CURRENT_SOURCE_DIR "stageData.txt");
        if (!(file.data())) { // 没有数据！
            cout << "stage file could not be read." << endl;
            return;
        }
        gState = new State(file.data(), file.size());
        // 第一绘制
        gState->draw();
        return; // 结束
    }
    bool cleared = false;
    // 主循环
    // 清除检测
    if (gState->hasCleared()) { cleared = true; }
    // 获取输入
    cout << "a:left s:right w:up z:down. command?" << endl; // 操作说明
    char input;
    cin >> input;
    // 结束判断
    if (input == 'q') {
        delete gState;
        gState = 0;
        Framework::instance().requestEnd();
        return;
    }
    // 更新
    gState->update(input);
    // 绘制
    gState->draw();

    if (cleared) {
        // 庆祝消息
        cout << "Congratulation! you win." << endl;
        delete gState;
        gState = 0;
    }
}
