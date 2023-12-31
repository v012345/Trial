#include "../lua/src/lua.hpp"
// 防止自动格式换位置
#include <stdlib.h>
#include <string>
#include <thread>
extern "C" {
int luaopen_socket_core(lua_State* L);
int luaopen_lfs(lua_State* L);
int luaopen_mime_core(lua_State* L);
}

void call_restart_bat() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_lfs(L);
    luaL_requiref(L, "socket.core", luaopen_socket_core, 0);
    lua_pop(L, 1);
    luaL_requiref(L, "mime.core", luaopen_mime_core, 0);
    lua_pop(L, 1);
    luaL_dofile(L, "update_restart.lua");
}

int main(int argc, char const* argv[]) {
#ifdef _WIN32
    system("chcp 65001 > NUL");
#endif
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_lfs(L);
    luaL_requiref(L, "socket.core", luaopen_socket_core, 0);
    lua_pop(L, 1);
    luaL_requiref(L, "mime.core", luaopen_mime_core, 0);
    lua_pop(L, 1);
    std::thread t(call_restart_bat);
    t.join();
    return 0;
}
