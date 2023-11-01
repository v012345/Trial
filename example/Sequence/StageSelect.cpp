#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Image.h"
#include "Sequence/Parent.h"
#include "Sequence/StageSelect.h"

namespace Sequence {

    StageSelect::StageSelect() : mImage(0) { mImage = new Image(CMAKE_CURRENT_SOURCE_DIR "data/image/stageSelect.dds"); }

    StageSelect::~StageSelect() { SAFE_DELETE(mImage); }

    void StageSelect::update(Parent* parent) {
        // 获取输入
        int stage = 0;
        // 如果下标移动了，那就麻烦了，所以放0
        char numberChars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        for (int i = 0; i < 10; ++i) {
            if (Framework::instance().isKeyTriggered(numberChars[i])) { stage = i; }
        }
        if (stage != 0) { // 到下一个
            parent->setStageID(stage);
            parent->moveTo(Parent::SEQ_GAME);
        }
        // 绘制
        mImage->draw();
    }

} // namespace Sequence