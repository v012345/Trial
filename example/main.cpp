#include "Games/Framework.h"
#include "Image.h"
using namespace GameLib;

#include "Pad.h"
#include "Sequence/Parent.h"

static int lua_image_draw(lua_State* L) {
    Image** image = (Image**)lua_touserdata(L, 1);
    (*image)->draw();
    return 0;
}
static int lua_image_gc(lua_State* L) {
    Image** image = (Image**)lua_touserdata(L, 1);
    delete *image;
    *image = nullptr;
    image = nullptr;
    return 0;
}
static int lua_create_image_instance(lua_State* L) {
    const char* filename = lua_tostring(L, 2);
    Image** image = (Image**)lua_newuserdata(L, sizeof(Image**));
    *image = new Image(filename);
    lua_newtable(L);
    lua_pushstring(L, "__index");
    luaL_getmetatable(L, "class_image_metatable");
    lua_settable(L, -3);
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, lua_image_gc);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    return 1;
}

static int luaopen_Image(lua_State* L) {
    luaL_Reg class_image_metatable[] = {
        {"draw", lua_image_draw}, //
        {NULL, NULL},
    };

    luaL_newmetatable(L, "class_image_metatable");
    luaL_setfuncs(L, class_image_metatable, 0);
    lua_pop(L, 1);

    lua_newtable(L);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, lua_create_image_instance);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_setglobal(L, "Image");
    return 1;
}
static int luaopen_framework(lua_State* L) {
    luaopen_Image(L);
    return 1;
}

// 用户封装函数。内容在Sequence :: Parent
namespace GameLib {
    void Framework::update() {
        if (!this->L) {
            this->L = luaL_newstate();
            luaopen_framework(this->L);
        }

        if (!Sequence::Parent::instance()) {
            Sequence::Parent::create();
            setFrameRate(60);
        }
        Sequence::Parent::instance()->update();
        // 结束判断（是否按下q或用鼠标按下X按钮）
        if (Pad::isOn(Pad::Q)) { requestEnd(); }
        if (isEndRequested()) { Sequence::Parent::destroy(); }
    }
} // namespace GameLib
