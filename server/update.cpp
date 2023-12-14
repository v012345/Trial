#include "../lua/src/lua.hpp"
// 防止自动格式换位置
#include <stdlib.h>
#include <string>
#include <thread>
extern "C" {
int luaopen_socket_core(lua_State* L);
int luaopen_lfs(lua_State* L);
int luaopen_mime_core(lua_State* L);
int luaopen_ssl_core(lua_State* L);
int luaopen_ssl_context(lua_State* L);
int luaopen_ssl_x509(lua_State* L);
int luaopen_ssl_config(lua_State* L);
}

void call_restart_bat() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    system("update_restart.bat");
}

int lua_call_restart_bat(lua_State* L) {
    std::thread t(call_restart_bat);
    t.detach();
    return 0;
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
    luaL_requiref(L, "ssl.core", luaopen_ssl_core, 0);
    lua_pop(L, 1);
    luaL_requiref(L, "ssl.context", luaopen_ssl_context, 0);
    lua_pop(L, 1);
    luaL_requiref(L, "ssl.x509", luaopen_ssl_x509, 0);
    lua_pop(L, 1);
    luaL_requiref(L, "ssl.config", luaopen_ssl_config, 0);
    lua_pop(L, 1);
    lua_register(L, "call_restart_bat", lua_call_restart_bat);
    luaL_dofile(L, "update.lua");
    return 0;
}
