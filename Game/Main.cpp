#include "Engine/Framework.h"
#include "libs/libpng/png.h"
#include "lua/src/lua.hpp"
#include <vector>
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
    namespace {
        static int powerOfTwo(int a) {
            ASSERT(a < 0x40000000); // 如果a大于0x40000000，则会导致无限循环。
            int r = 1;
            while (r < a) { r *= 2; }
            return r;
        }
        class Image {
          public:
            Image(const char* filename) : mWidth(0), mHeight(0), mTextureWidth(0), mTextureHeight(0), mTexture(0) {
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

                // 复制到临时缓冲区。直接从char *创建createTexture太危险。
                unsigned* buffer = new unsigned[mWidth * mHeight];
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
                        buffer[y * width + x] = j;
                    }
                }
                // createTexture。
                // 为此，必须找到一个为2的n次幂的分辨率。
                mTextureWidth = powerOfTwo(mWidth);
                mTextureHeight = powerOfTwo(mHeight);

                Framework::instance().createTexture(&mTexture, mTextureWidth, mTextureHeight, buffer, mWidth, mHeight);
                SAFE_DELETE_ARRAY(buffer); // 不再需要了

                fclose(fp);
                for (int y = 0; y < height; y++) { free(row_pointers[y]); }
                free(row_pointers);
                png_destroy_read_struct(&png, &info, NULL);
            }
            ~Image() { Framework::instance().destroyTexture(&mTexture); }
            int width() const { return mWidth; }
            int height() const { return mHeight; }
            void draw(int dstX, int dstY, int srcX, int srcY, int width, int height) const {
                // 计算x，y范围
                double x0 = static_cast<double>(dstX);
                double y0 = static_cast<double>(dstY);
                double x1 = x0 + static_cast<double>(width);
                double y1 = y0 + static_cast<double>(height);
                // 移动后的顶点
                std::vector<double> p0{x0, y0};
                std::vector<double> p1{x1, y0};
                std::vector<double> p2{x0, y1};
                std::vector<double> p3{x1, y1};
                // 纹理坐标生成
                double rcpTw = 1.0 / static_cast<double>(mTextureWidth);
                double rcpTh = 1.0 / static_cast<double>(mTextureHeight);
                double u0 = static_cast<double>(srcX) * rcpTw;
                double u1 = static_cast<double>(srcX + width) * rcpTw;
                double v0 = static_cast<double>(srcY) * rcpTh;
                double v1 = static_cast<double>(srcY + height) * rcpTh;
                std::vector<double> t0{u0, v0};
                std::vector<double> t1{u1, v0};
                std::vector<double> t2{u0, v1};
                std::vector<double> t3{u1, v1};

                Framework f = Framework::instance();
                // 纹理集
                f.setTexture(mTexture);
                // 线性合成
                f.setBlendMode(Framework::BLEND_LINEAR);
                // 绘制
                f.drawTriangle2D(&p0[0], &p1[0], &p2[0], &t0[0], &t1[0], &t2[0]);
                f.drawTriangle2D(&p3[0], &p1[0], &p2[0], &t3[0], &t1[0], &t2[0]);
            }
            void draw() const { draw(0, 0, 0, 0, mWidth, mHeight); }

          private:
            int mWidth;
            int mHeight;
            int mTextureWidth;
            int mTextureHeight;
            GameLib::Texture* mTexture;
        };
        static int lua_FrameworkTime(lua_State* L) {
            lua_Integer time = Framework::instance().time();
            lua_pushinteger(L, time);
            return 1;
        }
        static int luaopen_Framework(lua_State* L) {
            luaL_Reg funcs[] = {
                {"time", lua_FrameworkTime}, //
                {NULL, NULL}};
            luaL_newlib(L, funcs);
            return 1;
        }

        lua_State* L = nullptr;
        Image* image = nullptr;

    } // namespace

    void Framework::update() {
        if (!L) {
            L = luaL_newstate();
            luaL_openlibs(L);
            luaL_requiref(L, "Framework", luaopen_Framework, 1);
            luaL_dofile(L, CMAKE_SOURCE_DIR "scripts/Main.lua");
            image = new Image(CMAKE_SOURCE_DIR "res/dummy-origin.png");
        }
        image->draw();
        // lua_State* L = getLuaState();
        lua_getglobal(L, "MainLoop");
        lua_pcall(L, 0, 0, 0);
    }
} // namespace GameLib
