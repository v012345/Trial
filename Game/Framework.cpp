#include "Framework.h"
#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Mouse.h"

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
    {"frameRate", lua_frameRate}, //
    {"setFrameRate", lua_setFrameRate}, //
    {"isKeyTriggered", lua_isKeyTriggered}, //
    {"drawDebugString", lua_drawDebugString}, //
    {"mouse", lua_mouse}, //
    {"setVideoMemory", lua_setVideoMemory}, //
    {NULL, NULL},
};

int Framework::lua_setVideoMemory(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    unsigned c = luaL_checkinteger(L, 4);
    unsigned* vram = f.videoMemory();
    int ww = f.width(); // window width
    int wh = f.height(); // window height
    vram[y * ww + x] = c;
    return 0;
}

int Framework::lua_drawDebugString(lua_State* L) {
    int c = luaL_checkinteger(L, 2);
    int r = luaL_checkinteger(L, 3);
    const char* s = luaL_checkstring(L, 4);
    unsigned col = luaL_optinteger(L, 5, 0xffffffff);
    GameLib::Framework f = GameLib::Framework::instance();
    f.drawDebugString(c, r, s, col);
    return 0;
}

int Framework::lua_frameRate(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    lua_pushinteger(L, f.frameRate());
    return 1;
}
int Framework::lua_setFrameRate(lua_State* L) {
    int frameRate = luaL_checkinteger(L, 2);
    GameLib::Framework f = GameLib::Framework::instance();
    f.setFrameRate(frameRate);
    return 0;
}
int Framework::lua_isKeyTriggered(lua_State* L) {
    int c = luaL_checkinteger(L, 2);
    GameLib::Framework f = GameLib::Framework::instance();
    // lua_pushboolean(L, f.isKeyTriggered(c));
    return 1;
}

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
    // lua_pushboolean(L, f.isKeyOn(c));
    return 1;
}

int Framework::lua_mouse(lua_State* L) {
    GameLib::Input::Mouse mouse = GameLib::Input::Manager::instance().mouse();
    lua_newtable(L);
    int x = mouse.x();
    int y = mouse.y();
    bool LEFT = mouse.isOn(GameLib::Input::Mouse::BUTTON_LEFT);
    bool RIGHT = mouse.isOn(GameLib::Input::Mouse::BUTTON_RIGHT);
    bool MIDDLE = mouse.isOn(GameLib::Input::Mouse::BUTTON_MIDDLE);
    lua_pushstring(L, "x");
    lua_pushinteger(L, x);
    lua_settable(L, -3);
    lua_pushstring(L, "y");
    lua_pushinteger(L, y);
    lua_settable(L, -3);
    lua_pushstring(L, "left");
    lua_pushboolean(L, LEFT);
    lua_settable(L, -3);
    lua_pushstring(L, "right");
    lua_pushboolean(L, RIGHT);
    lua_settable(L, -3);
    lua_pushstring(L, "middle");
    lua_pushboolean(L, MIDDLE);
    lua_settable(L, -3);
    return 1;
}
