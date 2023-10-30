cmake_minimum_required(VERSION 3.20)
project(Trial)
add_compile_definitions(USE_WIN32_CONSOLE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory("libs")
add_subdirectory("lua/src")
add_subdirectory("extensions")

# add_subdirectory("tools")
add_subdirectory("Games")
add_subdirectory("example")

# add_definitions(-DNDEBUG)
add_executable(Main main.cpp)
target_link_libraries(Main
    liblua
    lfs
    png_shared
    freetype
)

# 把是显示找不到, 所以直接写出路径了
# target_include_directories(Main PRIVATE lua/src)
target_compile_definitions(Main PRIVATE
    LUA_MAIN_SCRIPT="${CMAKE_SOURCE_DIR}/job/main.lua"
    LUA_PACKAGE_PATH="${CMAKE_SOURCE_DIR}/job/?.lua"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
)

add_executable(Game WIN32
    "game.cpp"
    "Games/src/WindowCreator/WindowCreator.cpp"
)
target_include_directories(Game PRIVATE
    "Games/src"
    "Games/include"
)

target_link_libraries(Game
    GameLib
    liblua
    lfs
    png_shared
    freetype
)
