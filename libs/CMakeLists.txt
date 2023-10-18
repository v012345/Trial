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