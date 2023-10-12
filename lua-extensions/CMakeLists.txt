cmake_minimum_required(VERSION 3.20)
add_library(lfs SHARED lfs.c)
target_include_directories(lfs PRIVATE ${CMAKE_SOURCE_DIR}/lua-5.4.6/src)
target_link_libraries(lfs liblua-5.4.6)