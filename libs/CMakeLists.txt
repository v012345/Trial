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
set(ZLIB_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/zlib;${CMAKE_CURRENT_SOURCE_DIR}/include")
set(ZLIB_LIBRARIES zlib)
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

add_subdirectory(freetype)