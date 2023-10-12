#include "main.hpp"
int main(int argc, char const* argv[]) {
#ifdef _WIN32
    system("chcp 65001 > NUL");
#endif
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
#ifdef LUA_MAIN_SCRIPT
    luaL_dofile(L, LUA_MAIN_SCRIPT);
#endif
    return 0;
}
