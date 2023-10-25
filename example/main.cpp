#include "Games/Framework.h"
using namespace GameLib;

#include "Pad.h"
#include "Sequence/Parent.h"

// 用户封装函数。内容在Sequence :: Parent
namespace GameLib {
    void Framework::update() {
        if (!Sequence::Parent::instance()) {
            Sequence::Parent::create();
            setFrameRate(60);
        }
        Sequence::Parent::instance()->update();
        // 结束判断（是否按下q或用鼠标按下X按钮）
        if (Pad::isOn(Pad::Q)) { requestEnd(); }
        if (isEndRequested()) { Sequence::Parent::destroy(); }
    }
} // namespace GameLib
