cmake_minimum_required(VERSION 3.20)
project(Trial)
add_compile_definitions(USE_WIN32_CONSOLE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# add_definitions(-DNDEBUG)
add_subdirectory("libs")
add_subdirectory("lua/src")
add_subdirectory("extensions")
add_subdirectory("Game")
add_subdirectory("Engine")
add_subdirectory("Example")
add_subdirectory("windows")
add_subdirectory("server")

add_executable(Main main.cpp)
target_link_libraries(Main
    liblua
    lfs
    luasocket
    png_shared
    freetype
)
target_compile_definitions(Main PRIVATE
    LUA_MAIN_SCRIPT="${CMAKE_CURRENT_SOURCE_DIR}/main.lua"
    LUA_PACKAGE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/scripts/?.lua"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
)
