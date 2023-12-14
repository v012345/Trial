cmake_minimum_required(VERSION 3.20)
project(libs)

include_directories(include)

# 如果是第一个构建可以使用 add_subdirectory(zlib)
# 不然就用 add_library(zlib SHARED ...) 因为 zlib
# 是作为 submodule 引入的 但是在构建过程中会改变(删除 zconf.h 文件)
# add_subdirectory(zlib)
add_library(zlib SHARED
    zlib/adler32.c
    zlib/compress.c
    zlib/crc32.c
    zlib/deflate.c
    zlib/gzclose.c
    zlib/gzlib.c
    zlib/gzread.c
    zlib/gzwrite.c
    zlib/inflate.c
    zlib/infback.c
    zlib/inftrees.c
    zlib/inffast.c
    zlib/trees.c
    zlib/uncompr.c
    zlib/zutil.c
    zlib/win32/zlib1.rc
)
set_target_properties(zlib PROPERTIES DEFINE_SYMBOL ZLIB_DLL)

# 给 libpng 的设置
set(PNG_TOOLS OFF CACHE BOOL "" FORCE)
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(SKIP_INSTALL_ALL TRUE)
set(PNG_BUILD_ZLIB TRUE CACHE BOOL "Custom zlib location" FORCE)
target_include_directories(zlib PUBLIC zlib)
add_library(ZLIB::ZLIB ALIAS zlib)
add_subdirectory(libpng)

# 给 freetype 的设置
set(FT_DISABLE_HARFBUZZ TRUE CACHE BOOL "FT_DISABLE_HARFBUZZ" FORCE)
set(FT_DISABLE_PNG TRUE CACHE BOOL "FT_DISABLE_PNG" FORCE)
set(FT_DISABLE_ZLIB TRUE CACHE BOOL "FT_DISABLE_ZLIB" FORCE)
set(FT_DISABLE_BZIP2 TRUE CACHE BOOL "FT_DISABLE_BZIP2" FORCE)
set(FT_DISABLE_BROTLI TRUE CACHE BOOL "FT_DISABLE_BROTLI" FORCE)

if(WIN32)
    # 没看到 win32 有这个, 先设置成 "", 少一条警告
    set(PkgConfig "")
endif()

# 使用 freetype 来支持字体文件的解析
add_subdirectory(freetype)

# 下面是 openGL 的两大组合
add_subdirectory(glfw)

# glad 与 glew 二选一, 因为 glew 的 cmake 配置不明白, 所以使用 glad
# add_subdirectory(glad)

# 这里重要说一下, 直接从 git clone 或 submodule 到的没有 include 与 src, 所以要去 git 上去下载发行版, 目前是 2.2.0 版
# add_subdirectory(glew/build/cmake)
add_subdirectory(glew-2.2.0/build/cmake)

add_subdirectory(glm)
add_subdirectory(SOIL2)

# dear gui
add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PRIVATE imgui)
target_link_libraries(imgui glfw)
