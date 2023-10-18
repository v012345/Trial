#include "Games/Framework.h"

namespace GameLib {
    void Framework::update() {
        lua_State* L = getLuaState();
        lua_getglobal(L, "MainLoop");
        lua_pcall(L, 0, 0, 0);
    }
} // namespace GameLib
