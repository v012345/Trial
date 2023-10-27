#include "GameLib/GameLib.h"
#include "Games/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Play.h"
#include "Sequence/Title.h"

namespace Sequence {
    namespace Game {

        Pause::Pause() : mImage(0), mCursorPosistion(0) { mImage = new Image(CMAKE_CURRENT_SOURCE_DIR "data/image/dummy.dds"); }

        Pause::~Pause() { SAFE_DELETE(mImage); }

        Base* Pause::update(Parent* parent) {
            Base* next = this;
            // 0：重做
            // 1：面对面硒
            // 2:进入标题画面
            // 3：返回
            if (Pad::isTriggered(Pad::U)) {
                --mCursorPosistion;
                if (mCursorPosistion < 0) { //
                    mCursorPosistion = 1;
                }
            } else if (Pad::isTriggered(Pad::D)) {
                ++mCursorPosistion;
                if (mCursorPosistion > 1) { // 1越过0就循环成0
                    mCursorPosistion = 0;
                }
            } else if (Pad::isTriggered(Pad::A)) {
                if (mCursorPosistion == 0) { // 返回游戏
                    next = new Play;
                } else if (mCursorPosistion == 1) { // 返回主题
                    next = new Title;
                }
            }
            // 绘制
            // 首先是游戏画面
            parent->drawState();
            //
            mImage->draw();
            // 绘制文字
            Framework f = Framework::instance();
            f.drawDebugString(0, 0, "[pause]");
            f.drawDebugString(1, 2, "game continue");
            f.drawDebugString(1, 3, "return title");
            // 写光标
            f.drawDebugString(0, mCursorPosistion + 2, ">");

            return next;
        }

    } // namespace Game
} // namespace Sequence