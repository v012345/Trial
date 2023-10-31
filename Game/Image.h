#pragma once
#include "lua.hpp"
class Image {
  public:
    Image(const char* filename);
    ~Image();
    int width() const;
    int height() const;
    void draw(int dstX, int dstY, int srcX, int srcY, int width, int height) const;

    static int luaopen_Image(lua_State* L);
    static int lua_Image(lua_State* L);
    static int lua_gcImage(lua_State* L);

    static int lua_width(lua_State* L);
    static int lua_height(lua_State* L);
    static int lua_draw(lua_State* L);

  private:
    int mWidth;
    int mHeight;
    unsigned* mData;
    static luaL_Reg lua_reg[4];
};