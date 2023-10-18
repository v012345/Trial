#include "../Games/Framework.h"
#include "Image.h"

bool gFirst = true;
Image* gForeground;
Image* gBackground;

namespace GameLib {
    void Framework::update() {
        if (gFirst) {
            gFirst = false;
            gForeground = new Image(CMAKE_CURRENT_SOURCE_DIR "forground.dds");
            gBackground = new Image(CMAKE_CURRENT_SOURCE_DIR "background.dds");
        }
        // 用黑色填充屏幕（如果使用叠加则会和之前的帧内容相加最终变白）
        unsigned* vram = Framework::instance().videoMemory();
        int ww = Framework::instance().width();
        int wh = Framework::instance().height();
        for (int i = 0; i < ww * wh; ++i) { vram[i] = 0; }

        gBackground->draw(0, 0, 0, 0, gBackground->width(), gBackground->height());
        gForeground->draw(0, 0, 0, 0, gForeground->width(), gForeground->height());
    }
} // namespace GameLib
