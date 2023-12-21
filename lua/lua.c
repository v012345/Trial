#include "lua.h"

#include "lualib.h"

#include "lauxlib.h"
#include <stdlib.h>
int luaopen_lfs(lua_State* L);
int main(int argc, char const* argv[]) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_lfs(L);
#ifdef LUA_PACKAGE_PATH
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path");
    const char* path = lua_tostring(L, -1);
    char* new_path = (char*)malloc(sizeof(char) * (strlen(path) + strlen(";" LUA_PACKAGE_PATH) + 1));
    memcpy(new_path, path, strlen(path));
    memcpy(new_path + strlen(path), ";" LUA_PACKAGE_PATH, strlen(";" LUA_PACKAGE_PATH) + 1);
    lua_pop(L, 1);
    lua_pushstring(L, "path");
    lua_pushstring(L, new_path);
    lua_settable(L, -3);
    lua_pop(L, 1);
    free(new_path);
#endif
#ifdef CMAKE_SOURCE_DIR
    lua_pushstring(L, CMAKE_SOURCE_DIR);
    lua_setglobal(L, "CMAKE_SOURCE_DIR");
#endif
#ifdef LUA_MAIN_SCRIPT
    luaL_dofile(L, LUA_MAIN_SCRIPT);
#endif
    return 0;
}
