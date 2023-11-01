#pragma once
#include "Image.h"
#include "lua.hpp"

// 字符串绘制类
class StringRenderer {
  public:
    static int luaopen_StringRenderer(lua_State* L);

  private:
    static int lua_draw(lua_State* L);
    static Image* font;
    static luaL_Reg lua_reg[2];
};