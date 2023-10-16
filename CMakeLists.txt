cmake_minimum_required(VERSION 3.20)
project(Trial)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory("lua-5.4.6/src")
add_subdirectory("lua-extensions")
add_subdirectory("tools")
add_subdirectory("Games")

# add_definitions(-DNDEBUG)
add_executable(function_test main.cpp)
target_link_libraries(function_test
    liblua-5.4.6
    lfs
)
target_include_directories(function_test PRIVATE
    lua-5.4.6/src
)
target_compile_definitions(function_test PRIVATE
    LUA_MAIN_SCRIPT="${CMAKE_SOURCE_DIR}/scripts/main.lua"
    LUA_PACKAGE_PATH="${CMAKE_SOURCE_DIR}/scripts/?.lua"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)
add_compile_definitions(USE_WIN32_CONSOLE)
add_executable(Game WIN32
    "update.cpp"
    "Games/src/WindowCreator/WindowCreator.cpp"
)
target_include_directories(Game PRIVATE
    "Games/src"
    "Games/include"
)

target_link_libraries(Game
    GameLib
)

add_executable(Test WIN32
    "test.cpp"
    "Games/src/WindowCreator/WindowCreator.cpp"
)
target_include_directories(Test PRIVATE
    "Games/src"
    "Games/include"
)

target_link_libraries(Test
    GameLib
)

# add_custom_command(TARGET Test POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#     "${CMAKE_SOURCE_DIR}/log/stageData.txt"
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
