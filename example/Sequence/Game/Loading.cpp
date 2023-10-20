#include "GameLib/GameLib.h"
#include "Games/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Sequence/Game/Loading.h"
#include "Sequence/Game/Parent.h"

namespace Sequence {
    namespace Game {

        // 由于加载是在update开始的，因此加载不在此处开始，
        // “加载中”的图像正在加载
        Loading::Loading() : mImage(0), mCount(0), mStarted(false) { mImage = new Image(CMAKE_CURRENT_SOURCE_DIR "data/image/loading.dds"); }

        Loading::~Loading() { SAFE_DELETE(mImage); }

        void Loading::update(Parent* parent) {
            if (!mStarted) {
                //
                parent->startLoading();
                mStarted = true;
            } else if (mCount == 60) { // 1等待秒
                parent->moveTo(Parent::SEQ_PLAY); // 迁移到Play
            }

            // 绘制
            mImage->draw();

            ++mCount;
        }

    } // namespace Game
} // namespace Sequence