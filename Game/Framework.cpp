#include "Framework.h"
#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Mouse.h"
#include "Image.h"

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
    {"createTexture", lua_createTexture}, //
    {"setTexture", lua_setTexture}, //
    {"setBlendMode", lua_setBlendMode}, //
    {"drawTriangle3D", lua_drawTriangle3D}, //
    {"enableDepthTest", lua_enableDepthTest},
    {"enableDepthWrite", lua_enableDepthWrite},
    {NULL, NULL},
};
int Framework::lua_enableDepthWrite(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    bool b = lua_toboolean(L, 2);
    f.enableDepthWrite(b);
    return 0;
}
int Framework::lua_enableDepthTest(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    bool b = lua_toboolean(L, 2);
    f.enableDepthTest(b);
    return 0;
}
int Framework::lua_drawTriangle3D(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    double p[3][3];
    for (size_t i = 0; i < 3; i++) {
        luaL_checktype(L, i + 2, LUA_TTABLE);
        for (size_t j = 0; j < 3; j++) {
            lua_rawgeti(L, i + 2, j + 1);
            p[i][j] = luaL_checknumber(L, -1);
            lua_pop(L, 1);
        }
    }
    double* t[3] = {nullptr, nullptr, nullptr};
    for (size_t i = 0; i < 3; i++) {
        if (lua_type(L, i + 5) == LUA_TTABLE) {
            double* tt = new double[2];
            for (size_t j = 0; j < 2; j++) {
                lua_rawgeti(L, i + 5, j + 1);
                tt[j] = luaL_checknumber(L, -1);
                lua_pop(L, 1);
            }
            t[i] = tt;
        }
    }
    double c1 = luaL_optnumber(L, 8, 0xffffffff);
    double c2 = luaL_optnumber(L, 9, 0xffffffff);
    double c3 = luaL_optnumber(L, 10, 0xffffffff);

    f.drawTriangle3D(p[0], p[1], p[2], t[0], t[1], t[2], c1, c2, c3);
    for (size_t i = 0; i < 3; i++) {
        if (t[i]) { delete[] t[i]; }
    }
    return 0;
}
int Framework::lua_setTexture(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    GameLib::Texture** texture = static_cast<GameLib::Texture**>(lua_touserdata(L, 2));
    f.setTexture(*texture);
    return 0;
}
int Framework::lua_setBlendMode(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    int mode = lua_tointeger(L, 2);
    GameLib::Framework::BlendMode a = static_cast<GameLib::Framework::BlendMode>(mode);
    f.setBlendMode(a);
    return 0;
}
static int lua_gcDestroyTexture(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    GameLib::Texture** texture = static_cast<GameLib::Texture**>(lua_touserdata(L, 1));
    f.destroyTexture(texture);
    return 0;
}

int Framework::lua_createTexture(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    const char* filename = lua_tostring(L, 2);
    GameLib::Texture** texture = static_cast<GameLib::Texture**>(lua_newuserdata(L, sizeof(GameLib::Texture*)));
    *texture = nullptr;
    // Image* pImage = new Image(filename);
    // f.createTexture(texture, pImage->width(), pImage->height(), pImage->data(), pImage->width(), pImage->height());
    // SAFE_DELETE(pImage);
    f.createTexture(texture,filename);
    lua_newtable(L);
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, lua_gcDestroyTexture);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    return 1;
}

int Framework::lua_drawTriangle2D(lua_State* L) {
    GameLib::Framework f = GameLib::Framework::instance();
    double p[3][2];
    for (size_t i = 0; i < 3; i++) {
        luaL_checktype(L, i + 2, LUA_TTABLE);
        for (size_t j = 0; j < 2; j++) {
            lua_rawgeti(L, i + 2, j + 1);
            p[i][j] = luaL_checknumber(L, -1);
            lua_pop(L, 1);
        }
    }

    double* t[3] = {nullptr, nullptr, nullptr};
    for (size_t i = 0; i < 3; i++) {
        if (lua_type(L, i + 5) == LUA_TTABLE) {
            double* tt = new double[2];
            for (size_t j = 0; j < 2; j++) {
                lua_rawgeti(L, i + 5, j + 1);
                tt[j] = luaL_checknumber(L, -1);
                lua_pop(L, 1);
            }
            t[i] = tt;
        }
    }
    double c1 = luaL_optnumber(L, 8, 0xffffffff);
    double c2 = luaL_optnumber(L, 9, 0xffffffff);
    double c3 = luaL_optnumber(L, 10, 0xffffffff);

    f.drawTriangle2D(p[0], p[1], p[2], t[0], t[1], t[2], c1, c2, c3);
    for (size_t i = 0; i < 3; i++) {
        if (t[i]) { delete[] t[i]; }
    }
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
    lua_pushboolean(L, f.isKeyTriggered(c));
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
    lua_pushboolean(L, f.isKeyOn(c));
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
