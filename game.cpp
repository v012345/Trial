#include "Games/Framework.h"
#include "lua/src/lua.hpp"
// static int lua_image_draw(lua_State* L) {
//     Image** image = (Image**)lua_touserdata(L, 1);
//     (*image)->draw();
//     return 0;
// }
// static int lua_image_gc(lua_State* L) {
//     Image** image = (Image**)lua_touserdata(L, 1);
//     delete *image;
//     *image = nullptr;
//     image = nullptr;
//     return 0;
// }
// static int lua_create_image_instance(lua_State* L) {
//     const char* filename = lua_tostring(L, 2);
//     Image** image = (Image**)lua_newuserdata(L, sizeof(Image**));
//     *image = new Image(filename);
//     lua_newtable(L);
//     lua_pushstring(L, "__index");
//     luaL_getmetatable(L, "class_image_metatable");
//     lua_settable(L, -3);
//     lua_pushstring(L, "__gc");
//     lua_pushcfunction(L, lua_image_gc);
//     lua_settable(L, -3);
//     lua_setmetatable(L, -2);
//     return 1;
// }

// static int luaopen_Image(lua_State* L) {
//     luaL_Reg class_image_metatable[] = {
//         {"draw", lua_image_draw}, //
//         {NULL, NULL},
//     };

//     luaL_newmetatable(L, "class_image_metatable");
//     luaL_setfuncs(L, class_image_metatable, 0);
//     lua_pop(L, 1);

//     lua_newtable(L);
//     lua_newtable(L);
//     lua_pushstring(L, "__call");
//     lua_pushcfunction(L, lua_create_image_instance);
//     lua_settable(L, -3);
//     lua_setmetatable(L, -2);
//     lua_setglobal(L, "Image");
//     return 1;
// }
// static int luaopen_framework(lua_State* L) {
//     luaopen_Image(L);
//     return 1;
// }
// if (!this->L) {
//     this->L = luaL_newstate();
//     luaopen_framework(this->L);
// }
namespace GameLib {
    namespace {} // namespace

    void Framework::update() {
        // lua_State* L = getLuaState();
        // lua_getglobal(L, "MainLoop");
        // lua_pcall(L, 0, 0, 0);
    }
} // namespace GameLib
