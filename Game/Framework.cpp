#include "Framework.h"
#include "GameLib/Framework.h"

int Framework::luaopen_Framework(lua_State* L) {
    lua_newtable(L);
    luaL_setfuncs(L, lua_reg, 0);
    lua_setglobal(L, "Framework");
    return 1;
}

luaL_Reg Framework::lua_reg[] = {
    {"time", lua_time}, //
    {"sleep", lua_sleep}, //
    {"isKeyOn", lua_isKeyOn}, //
    {NULL, NULL},
};

int Framework::lua_sleep(lua_State* L) {
    int ms = luaL_checkinteger(L, 2);
    GameLib::Framework f = GameLib::Framework::instance();
    f.sleep(ms);
    return 0;
}
int Framework::lua_time(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    lua_pushinteger(L, f.time());
    return 1;
}
int Framework::lua_isKeyOn(lua_State* L) {
    int c = luaL_checkinteger(L, 2);
    GameLib::Framework f = GameLib::Framework::instance();
    lua_pushboolean(L, f.isKeyOn(c));
    return 1;
}