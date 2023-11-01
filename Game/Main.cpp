#include "Framework.h"
#include "GameLib/Framework.h"
#include "Image.h"
#include "libpng/png.h"
#include "lua.hpp"
#include <vector>

static lua_State* L = nullptr;
extern int luaopen_Config(lua_State*);
namespace GameLib {
    void Framework::update() {
        if (!L) {
            L = luaL_newstate();
            luaL_openlibs(L);
            Image::luaopen_Image(L);
            ::Framework::luaopen_Framework(L);
            luaopen_Config(L);
            luaL_dofile(L, CMAKE_CURRENT_SOURCE_DIR "scripts/Main.lua");
        }
        lua_getglobal(L, "MainLoop");
        lua_pcall(L, 0, 0, 0);
    }
} // namespace GameLib
