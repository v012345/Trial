#include "lua.h"

#include "lualib.h"

#include "lauxlib.h"

int luaopen_lfs(lua_State* L);
int main(int argc, char const* argv[]) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_lfs(L);
    luaL_dofile(L, "main.lua");
    return 0;
}
