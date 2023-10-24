#include "GameLib/GameLib.h"
#include "Games/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"

namespace Sequence {

    Title::Title() : mImage(0), mCursorPosistion(0) { mImage = new Image(CMAKE_CURRENT_SOURCE_DIR "data/image/dummy.dds"); }

    Title::~Title() { SAFE_DELETE(mImage); }

    void Title::update(Parent* parent) {
        // 获取输入
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
            parent->moveTo(Parent::NEXT_GAME);
            if (mCursorPosistion == 0) {
                parent->setMode(Parent::MODE_1P);
            } else if (mCursorPosistion == 1) {
                parent->setMode(Parent::MODE_2P);
            } else {
                HALT("arienai");
            }
        }
        // 绘制
        mImage->draw();
        // 输出文字
        f.drawDebugString(0, 0, "[title] : boom man");
        f.drawDebugString(1, 2, "one player");
        f.drawDebugString(1, 3, "two players");
        // 写光标
        f.drawDebugString(0, mCursorPosistion + 2, ">");
    }

} // namespace Sequence