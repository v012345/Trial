#include "GameLib/GameLib.h"
#include "Games/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Sequence/Game/Judge.h"
#include "Sequence/Game/Parent.h"

namespace Sequence {
    namespace Game {

        Judge::Judge() : mImage(0), mCursorPosistion(0) { mImage = new Image(CMAKE_CURRENT_SOURCE_DIR "data/image/dummy.dds"); }

        Judge::~Judge() { SAFE_DELETE(mImage); }

        void Judge::update(Parent* parent) {
            Framework f = Framework::instance();
            ;
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
                if (mCursorPosistion == 0) {
                    //
                    parent->moveTo(Parent::NEXT_READY);
                } else if (mCursorPosistion == 1) {
                    // 到主题
                    parent->moveTo(Parent::NEXT_TITLE);
                }
            }
            // 绘制
            // 首先是游戏画面
            parent->drawState();
            //
            mImage->draw();

            // 显示哪个赢了
            f.drawDebugString(0, 0, "[show result]");

            Parent::PlayerID winner = parent->winner();
            if (winner == Parent::PLAYER_1) {
                f.drawDebugString(0, 1, "1P win!");
            } else if (winner == Parent::PLAYER_2) {
                f.drawDebugString(0, 1, "2P win!");
            } else {
                f.drawDebugString(0, 1, "nobody win");
            }
            // 菜单
            f.drawDebugString(1, 3, "game continue");
            f.drawDebugString(1, 4, "return title");
            // 写光标
            f.drawDebugString(0, mCursorPosistion + 3, ">");
        }

    } // namespace Game
} // namespace Sequence