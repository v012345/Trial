#include "StringRenderer.h"
#include "GameLib/Framework.h"

Image* StringRenderer::font = new Image(CMAKE_CURRENT_SOURCE_DIR "res/font.png");
int StringRenderer::lua_draw(lua_State* L) {
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);
    const char* s = luaL_checkstring(L, 4);
    int c = luaL_optinteger(L, 5, 0xffffff);
    // 两个常数。
    const int CHAR_WIDTH = 8;
    const int CHAR_HEIGHT = 16;
    // 初始化写入目的地
    int dstX = x * CHAR_WIDTH;
    int dstY = y * CHAR_HEIGHT;
    // 获取框架
    GameLib::Framework f = GameLib::Framework::instance();
    // 范围检查
    if (dstY < 0 || dstY >= (int)f.height()) { return 0; }
    // 图像宽度
    for (int i = 0; s[i] != '\0'; ++i) {
        // X范围判断
        if (dstX >= 0 && (dstX + CHAR_WIDTH) < f.width()) {
            // 字符范围检查
            int t = s[i];
            if (t < 32 || t >= 128) {
                t = 127; //
            }
            t -= 32; // 减去32
            int srcX = (t % 16) * CHAR_WIDTH; // X是除以宽度的余数
            int srcY = (t / 16) * CHAR_HEIGHT; // Y是除以宽度后的商
            font->drawWithFixedColor(dstX, dstY, srcX, srcY, CHAR_WIDTH, CHAR_HEIGHT, c);
        }
        dstX += CHAR_WIDTH;
    }
    return 0;
}

int StringRenderer::luaopen_StringRenderer(lua_State* L) {
    lua_newtable(L);
    luaL_setfuncs(L, lua_reg, 0);
    lua_setglobal(L, "StringRenderer");
    return 1;
}

luaL_Reg StringRenderer::lua_reg[] = {
    {"draw", lua_draw}, //
    {NULL, NULL},
};