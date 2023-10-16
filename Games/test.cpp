#include "Framework.h"

namespace GameLib {
    void Framework::update() {
        unsigned* vram = videoMemory();
        // static unsigned i;

        for (size_t i = 0; i < width() * height(); i++) { vram[i] = i; }

        // i += 9973; // 小于1万的最大的质数
        // i %= (width() * height());
    }
} // namespace GameLib
