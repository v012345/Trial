#include "Games/Framework.h"

namespace GameLib {
    void Framework::update() {
        lua_State* L = getLuaState();
        lua_getglobal(L, "MainLoop");
        lua_pcall(L, 0, 0, 0);
        // unsigned* vram = videoMemory();
        // static unsigned i;
        // vram[200 * width() + 100] = 0xFF0000;
        // for (size_t i = 0; i < width() * height(); i++) { vram[i] = 0xFF0000; }
        // for (size_t i = 0; i < width() * 3; i++) { vram[i] = 0xFFFFFF; }
        // i += 9973; // 小于1万的最大的质数
        // i %= (width() * height());
    }
} // namespace GameLib
