#include "libs/libpng/png.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "lua/src/lua.hpp"
// 防止自动格式换位置
#include "extensions/luafilesystem/src/lfs.h"
#include <conio.h>
#include <stdlib.h>
#include <string>
#include <windows.h>
static int Lua_SetConsoleCursorPosition(lua_State* L);
static int Lua__getch(lua_State* L);
static void read_png_file(const char* filename);

int main(int argc, char const* argv[]) {
    read_png_file(CMAKE_SOURCE_DIR "res/shengyue.png");
#ifdef _WIN32
    system("chcp 65001 > NUL");
#endif
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_lfs(L);
    lua_register(L, "SetConsoleCursorPosition", Lua_SetConsoleCursorPosition);
    lua_register(L, "_getch", Lua__getch);
#ifdef LUA_PACKAGE_PATH
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path");
    const char* path = lua_tostring(L, -1);
    char* new_path = (char*)malloc(sizeof(char) * (strlen(path) + strlen(";" LUA_PACKAGE_PATH) + 1));
    memcpy(new_path, path, strlen(path));
    memcpy(new_path + strlen(path), ";" LUA_PACKAGE_PATH, strlen(";" LUA_PACKAGE_PATH) + 1);
    lua_pop(L, 1);
    lua_pushstring(L, "path");
    lua_pushstring(L, new_path);
    lua_settable(L, -3);
    lua_pop(L, 1);
    free(new_path);
#endif
#ifdef CMAKE_SOURCE_DIR
    lua_pushstring(L, CMAKE_SOURCE_DIR);
    lua_setglobal(L, "CMAKE_SOURCE_DIR");
#endif
#ifdef LUA_MAIN_SCRIPT
    luaL_dofile(L, LUA_MAIN_SCRIPT);
#endif
    return 0;
}
static int Lua__getch(lua_State* L) {
    int c = _getch();
    lua_pushinteger(L, c);
    return 1;
}
static int Lua_SetConsoleCursorPosition(lua_State* L) {
    int column = lua_tointeger(L, 1);
    int line = lua_tointeger(L, 2);
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    return 0;
}

static void read_png_file(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Failed to open file %s for reading\n", filename);
        return;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        printf("png_create_read_struct failed\n");
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        fclose(fp);
        png_destroy_read_struct(&png, NULL, NULL);
        printf("png_create_info_struct failed\n");
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        fclose(fp);
        png_destroy_read_struct(&png, &info, NULL);
        printf("Error during init_io\n");
        return;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    int bit_depth = png_get_bit_depth(png, info);
    int color_type = png_get_color_type(png, info);

    printf("Image width: %d, height: %d\n", width, height);
    printf("Bit depth: %d, Color type: %d\n", bit_depth, color_type);

    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) { row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info)); }

    png_read_image(png, row_pointers);

    // Now you can access pixel values in the row_pointers array.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Access pixel data at (x, y)
            // Example for grayscale images:
            png_byte pixel_value = row_pointers[y][x];
        }
    }

    fclose(fp);
    for (int y = 0; y < height; y++) { free(row_pointers[y]); }
    free(row_pointers);
    png_destroy_read_struct(&png, &info, NULL);
}