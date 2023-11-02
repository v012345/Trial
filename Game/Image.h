#pragma once
struct lua_State;
struct luaL_Reg;
namespace GameLib {
    class Texture;
}
class Image {
  public:
    Image(const char* filename);
    ~Image();
    int width() const;
    int height() const;
    void draw() const;
    void draw(int dstX, int dstY, int srcX, int srcY, int width, int height) const;
    void drawWithFixedColor(int dstX, int dstY, int srcX, int srcY, int width, int height, unsigned color) const;

    static int luaopen_Image(lua_State* L);
    static int lua_Image(lua_State* L);
    static int lua_gcImage(lua_State* L);

    static int lua_width(lua_State* L);
    static int lua_height(lua_State* L);
    static int lua_draw(lua_State* L);

  private:
    int mWidth;
    int mHeight;
    int mTextureWidth;
    int mTextureHeight;
    GameLib::Texture* mTexture;
    static luaL_Reg lua_reg[4];
};