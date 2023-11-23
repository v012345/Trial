#pragma once
#include "lua.hpp"
class Framework {
  public:
    static int luaopen_Framework(lua_State* L);


    static int lua_sleep(lua_State* L);
    static int lua_isKeyOn(lua_State* L);
    static int lua_time(lua_State* L);
    static int lua_frameRate(lua_State* L);
    static int lua_setFrameRate(lua_State* L);
    static int lua_isKeyTriggered(lua_State* L);
    static int lua_drawDebugString(lua_State* L);
    static int lua_mouse(lua_State* L);
    static int lua_setVideoMemory(lua_State* L);
    static int lua_width(lua_State* L);
    static int lua_height(lua_State* L);
    static int lua_drawTriangle2D(lua_State* L);
    static int lua_createTexture(lua_State* L);
    static int lua_setTexture(lua_State* L);
    static int lua_setBlendMode(lua_State* L);
    static int lua_drawTriangle3D(lua_State* L);

  private:
    static luaL_Reg lua_reg[17];
};