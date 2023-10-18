cmake_minimum_required(VERSION 3.20)
project(libs)
set(PNG_TOOLS OFF)
set(PNG_TESTS OFF)
set(PNG_BUILD_ZLIB TRUE)
set(ZLIB_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/zlib;${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory(zlib)
add_subdirectory(libpng)

# 目前还不清楚为什么会出现这个问题
# 如果 Build 不出来, 那么就把下面这行注释了
# 之后多点几次 Build, 到出现 `png.obj : error LNK2019: unresolved external symbol _inflateReset` 为止
# 再把下面的注释解开, 再 Build 几次就成功了
# 我怀疑是 zlib 构建顺序的问题, 先构建出 zlib.lib 就行了
target_link_libraries(png_shared PUBLIC zlib)