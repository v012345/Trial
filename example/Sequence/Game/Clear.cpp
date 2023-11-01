#include "GameLib/Framework.h"
#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "State.h"

namespace Sequence {
    namespace Game {

        Clear::Clear() : mImage(0), mCount(0) { mImage = new Image(CMAKE_CURRENT_SOURCE_DIR "data/image/clear.dds"); }

        Clear::~Clear() { SAFE_DELETE(mImage); }

        void Clear::update(Parent* parent) {
            if (mCount == 60) { // 按fps=60移动
                parent->moveTo(Parent::SEQ_STAGE_SELECT);
            }

            // 绘制
            // 首先绘制游戏画面
            parent->state()->draw();
            // 在顶部绘制庆祝消息
            mImage->draw();

            ++mCount;
        }

    } // namespace Game
} // namespace Sequence
