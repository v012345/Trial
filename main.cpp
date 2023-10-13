#include "main.hpp"
static int Lua_SetConsoleCursorPosition(lua_State* L);
static int Lua__getch(lua_State* L);
int main(int argc, char const* argv[]) {
#ifdef _WIN32
    system("chcp 65001 > NUL");
#endif
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_lfs(L);
    lua_register(L, "SetConsoleCursorPosition", Lua_SetConsoleCursorPosition);
    lua_register(L, "_getch", Lua__getch);
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
static int Lua__getch(lua_State* L) {
    int c = _getch();
    lua_pushinteger(L, c);
    return 1;
}
static int Lua_SetConsoleCursorPosition(lua_State* L) {
    int column = lua_tointeger(L, 1);
    int line = lua_tointeger(L, 2);
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    return 0;
}