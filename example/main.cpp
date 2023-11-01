#include "GameLib/Framework.h"
#include "GameLib/Gamelib.h"
using namespace GameLib;

#include "File.h"
#include "Image.h"
#include "Sequence/Parent.h"
#include "State.h"

// 全局变量
Sequence::Parent* gRootSequence = 0; // 根序列
int gCounter = 0; // 一个计数器，用于计算主循环的次数

// 用户封装函数。内容被抛出给mainLoop（）
namespace GameLib {
    void Framework::update() {
        if (!gRootSequence) { gRootSequence = new Sequence::Parent(); }
        // 帧频调整
        setFrameRate(60); // 只需要调用一次

        if (gCounter % 60 == 0) { // 60每帧显示一次帧率
            cout << " FrameRate:" << frameRate() << endl;
        }
        ++gCounter;

        gRootSequence->update();

        // 结束判断（是否按下q或用鼠标按下X按钮）
        if (isKeyOn('q')) { requestEnd(); }
        if (isEndRequested()) { SAFE_DELETE(gRootSequence); }
    }
} // namespace GameLib
