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
        gBackground->draw(0, 0, 0, 0, gBackground->width(), gBackground->height());
        gForeground->draw(0, 0, 0, 0, gForeground->width(), gForeground->height());
    }
} // namespace GameLib
