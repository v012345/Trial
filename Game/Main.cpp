#include "GameLib/Framework.h"
#include "Image.h"
#include "libpng/png.h"
#include "lua.hpp"
#include <vector>

static lua_State* L = nullptr;

namespace GameLib {
    void Framework::update() {
        if (!L) {
            L = luaL_newstate();
            luaL_openlibs(L);
            Image::luaopen_Image(L);
            luaL_dofile(L, CMAKE_CURRENT_SOURCE_DIR "scripts/Main.lua");
        }
    }
} // namespace GameLib
