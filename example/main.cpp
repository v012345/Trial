#include "GameLib/Framework.h"

namespace GameLib {
    void Framework::update() {
        unsigned* vram = videoMemory();
        static unsigned i;
        vram[i] += i * 100;
        i += 9973; // 小于1万的最大的质数
        i %= (width() * height());
    }
} // namespace GameLib
