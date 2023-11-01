#include "Image.h"
#include "GameLib/Framework.h"
#include "libpng/png.h"
#include <stdlib.h>

Image::Image(const char* filename) : mWidth(0), mHeight(0), mData(0) {
    FILE* fp = fopen(filename, "rb");
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    png_init_io(png, fp);
    png_read_info(png, info);
    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    int bit_depth = png_get_bit_depth(png, info);
    int color_type = png_get_color_type(png, info);
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) { //
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }
    png_read_image(png, row_pointers);
    int bytes_per_pixel;
    if (color_type & PNG_COLOR_MASK_ALPHA) {
        bytes_per_pixel = 4; // The image has an alpha channel
    } else {
        bytes_per_pixel = 3; // The image does not have an alpha channel
    }
    mWidth = width;
    mHeight = height;
    mData = new unsigned[mWidth * mHeight];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned j = row_pointers[y][x * bytes_per_pixel] << 16;
            j += row_pointers[y][x * bytes_per_pixel + 1] << 8;
            j += row_pointers[y][x * bytes_per_pixel + 2];
            if (bytes_per_pixel == 4) { //
                j += row_pointers[y][x * bytes_per_pixel + 3] << 24;
            } else {
                j += 0xff000000;
            }
            mData[y * width + x] = j;
        }
    }
    fclose(fp);
    for (int y = 0; y < height; y++) { free(row_pointers[y]); }
    free(row_pointers);
    png_destroy_read_struct(&png, &info, NULL);
}

Image::~Image() {
    delete[] mData;
    mData = 0;
}

int Image::width() const { return mWidth; }

int Image::height() const { return mHeight; }

void Image::draw(int dstX, int dstY, int srcX, int srcY, int width, int height) const {
    unsigned* vram = GameLib::Framework::instance().videoMemory();
    unsigned windowWidth = GameLib::Framework::instance().width();
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned src = mData[(y + srcY) * mWidth + (x + srcX)];
            unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];
            unsigned srcA = (src & 0xff000000) >> 24;
            unsigned srcR = src & 0xff0000;
            unsigned srcG = src & 0x00ff00;
            unsigned srcB = src & 0x0000ff;
            unsigned dstR = *dst & 0xff0000;
            unsigned dstG = *dst & 0x00ff00;
            unsigned dstB = *dst & 0x0000ff;
            unsigned r = (srcR - dstR) * srcA / 255 + dstR;
            unsigned g = (srcG - dstG) * srcA / 255 + dstG;
            unsigned b = (srcB - dstB) * srcA / 255 + dstB;
            *dst = (r & 0xff0000) | (g & 0x00ff00) | b;
        }
    }
}

// 指定颜色绘制
void Image::drawWithFixedColor(int dstX, int dstY, int srcX, int srcY, int width, int height, unsigned color) const {
    unsigned* vram = GameLib::Framework::instance().videoMemory();
    int windowWidth = GameLib::Framework::instance().width();
    unsigned srcR = color & 0xff0000;
    unsigned srcG = color & 0x00ff00;
    unsigned srcB = color & 0x0000ff;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned src = mData[(y + srcY) * mWidth + (x + srcX)];
            unsigned* dst = &vram[(y + dstY) * windowWidth + (x + dstX)];
            unsigned srcA = (src & 0xff000000) >> 24;
            unsigned dstR = *dst & 0xff0000;
            unsigned dstG = *dst & 0x00ff00;
            unsigned dstB = *dst & 0x0000ff;
            unsigned r = (srcR - dstR) * srcA / 255 + dstR;
            unsigned g = (srcG - dstG) * srcA / 255 + dstG;
            unsigned b = (srcB - dstB) * srcA / 255 + dstB;
            *dst = (r & 0xff0000) | (g & 0x00ff00) | b;
        }
    }
}

int Image::luaopen_Image(lua_State* L) {
    lua_newtable(L);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, lua_Image);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_setglobal(L, "Image");
    return 1;
}

int Image::lua_Image(lua_State* L) {
    const char* filename = lua_tostring(L, 2);
    Image** image = static_cast<Image**>(lua_newuserdata(L, sizeof(Image*)));
    *image = new Image(filename);
    lua_newtable(L);
    lua_pushstring(L, "__index");
    lua_newtable(L);
    luaL_setfuncs(L, lua_reg, 0);
    lua_settable(L, -3);
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, lua_gcImage);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    return 1;
}

int Image::lua_gcImage(lua_State* L) {
    Image** image = static_cast<Image**>(lua_touserdata(L, 1));
    delete *image;
    *image = nullptr;
    image = nullptr;
    return 1;
}

luaL_Reg Image::lua_reg[] = {
    {"draw", lua_draw}, //
    {"width", lua_width}, //
    {"height", lua_height}, //
    {NULL, NULL},
};

int Image::lua_draw(lua_State* L) {
    Image* image = *(static_cast<Image**>(lua_touserdata(L, 1)));
    int dstX = luaL_optnumber(L, 2, 0);
    int dstY = luaL_optnumber(L, 3, 0);
    int srcX = luaL_optnumber(L, 4, 0);
    int srcY = luaL_optnumber(L, 5, 0);
    int width = luaL_optnumber(L, 6, image->width());
    int height = luaL_optnumber(L, 7, image->height());
    image->draw(dstX, dstY, srcX, srcY, width, height);
    return 0;
}
int Image::lua_width(lua_State* L) {
    Image** image = static_cast<Image**>(lua_touserdata(L, 1));
    lua_pushinteger(L, (*image)->width());
    return 1;
}
int Image::lua_height(lua_State* L) {
    Image** image = static_cast<Image**>(lua_touserdata(L, 1));
    lua_pushinteger(L, (*image)->height());
    return 1;
}