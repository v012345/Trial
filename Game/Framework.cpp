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
    {"width", lua_width}, //
    {"height", lua_height}, //
    {"drawTriangle2D", lua_drawTriangle2D}, //
    {NULL, NULL},
};
int Framework::lua_drawTriangle2D(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    // 从Lua中获取三个点的坐标（每个点是一个包含两个元素的数组）
    luaL_checktype(L, 2, LUA_TTABLE);
    luaL_checktype(L, 3, LUA_TTABLE);
    luaL_checktype(L, 4, LUA_TTABLE);

    // 读取数组中的元素值
    lua_rawgeti(L, 2, 1);
    double point1X = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_rawgeti(L, 2, 2);
    double point1Y = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_rawgeti(L, 3, 1);
    double point2X = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_rawgeti(L, 3, 2);
    double point2Y = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_rawgeti(L, 4, 1);
    double point3X = luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_rawgeti(L, 4, 2);
    double point3Y = luaL_checknumber(L, -1);
    lua_pop(L, 1);
    double t1 = luaL_optnumber(L, 5, 0);
    double t2 = luaL_optnumber(L, 6, 0);
    double t3 = luaL_optnumber(L, 7, 0);
    double c1 = luaL_optnumber(L, 8, 0xffffffff);
    double c2 = luaL_optnumber(L, 9, 0xffffffff);
    double c3 = luaL_optnumber(L, 10, 0xffffffff);

    double p0[2] = {point1X, point1Y};
    double p1[2] = {point2X, point2Y};
    double p2[2] = {point3X, point3Y};
    f.drawTriangle2D(p0, p1, p2, &t1, &t2, &t3, c1, c2, c3);
    return 0;
}

int Framework::lua_width(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    lua_pushinteger(L, f.width());
    return 1;
}
int Framework::lua_height(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    lua_pushinteger(L, f.height());
    return 1;
}

int Framework::lua_setVideoMemory(lua_State* L) {
    luaL_error(L, "plz not use this, use gpu~");
    // GameLib::Framework f = GameLib::Framework::instance();
    // int x = luaL_checkinteger(L, 2);
    // int y = luaL_checkinteger(L, 3);
    // unsigned c = luaL_checkinteger(L, 4);
    // unsigned* vram = f.videoMemory();
    // int ww = f.width(); // window width
    // int wh = f.height(); // window height
    // vram[y * ww + x] = c;
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
