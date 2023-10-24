#include "GameLib/GameLib.h"
#include "Games/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Pause.h"

namespace Sequence {
    namespace Game {

        Pause::Pause() : mImage(0), mCursorPosistion(0) { mImage = new Image(CMAKE_CURRENT_SOURCE_DIR "data/image/dummy.dds"); }

        Pause::~Pause() { SAFE_DELETE(mImage); }

        void Pause::update(Parent* parent) {
            Framework f = Framework::instance();
            ;
            // 0：重做
            // 1：面对面硒
            // 2:进入标题画面
            // 3：返回
            if (f.isKeyTriggered('w')) {
                --mCursorPosistion;
                if (mCursorPosistion < 0) { //
                    mCursorPosistion = 1;
                }
            } else if (f.isKeyTriggered('z')) {
                ++mCursorPosistion;
                if (mCursorPosistion > 1) { // 1越过0就循环成0
                    mCursorPosistion = 0;
                }
            } else if (f.isKeyTriggered(' ')) {
                if (mCursorPosistion == 0) { // 返回游戏
                    parent->moveTo(Parent::NEXT_PLAY);
                } else if (mCursorPosistion == 1) { // 返回主题
                    parent->moveTo(Parent::NEXT_TITLE);
                }
            }
            // 绘制
            // 首先是游戏画面
            parent->drawState();
            //
            mImage->draw();
            // 绘制文字
            f.drawDebugString(0, 0, "[pause]");
            f.drawDebugString(1, 2, "game continue");
            f.drawDebugString(1, 3, "return title");
            // 写光标
            f.drawDebugString(0, mCursorPosistion + 2, ">");
        }

    } // namespace Game
} // namespace Sequence