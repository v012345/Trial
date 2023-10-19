cmake_minimum_required(VERSION 3.20)
project(Trial)
add_compile_definitions(USE_WIN32_CONSOLE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory("libs")
add_subdirectory("lua-5.4.6/src")
add_subdirectory("lua-extensions")
add_subdirectory("tools")
add_subdirectory("Games")
add_subdirectory("example")

# add_definitions(-DNDEBUG)
add_executable(Main main.cpp)
target_link_libraries(Main
    liblua-5.4.6
    lfs
    png_shared
)
target_include_directories(Main PRIVATE
    lua-5.4.6/src
)
target_compile_definitions(Main PRIVATE
    LUA_MAIN_SCRIPT="${CMAKE_SOURCE_DIR}/job/main.lua"
    LUA_PACKAGE_PATH="${CMAKE_SOURCE_DIR}/job/?.lua"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
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
)
