#include "lua.hpp"
#include "stdlib.h"
#include <string>
extern "C" {
int luaopen_socket_core(lua_State* L);
int luaopen_mime_core(lua_State* L);
}

int luaopen_Config(lua_State* L) {
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
    luaL_requiref(L, "socket.core", luaopen_socket_core, 0);
    lua_pop(L, 1);
    luaL_requiref(L, "mime.core", luaopen_mime_core, 0);
    lua_pop(L, 1);
    lua_pushstring(L, CMAKE_CURRENT_SOURCE_DIR);
    lua_setglobal(L, "CMAKE_CURRENT_SOURCE_DIR");
    lua_pushstring(L, CMAKE_SOURCE_DIR);
    lua_setglobal(L, "CMAKE_SOURCE_DIR");
    return 1;
}
