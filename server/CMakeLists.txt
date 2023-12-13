cmake_minimum_required(VERSION 3.20)

add_executable(server server.cpp)
target_link_libraries(server
    liblua
    lfs
    luasocket
)
target_compile_definitions(server PRIVATE
    LUA_MAIN_SCRIPT="${CMAKE_CURRENT_SOURCE_DIR}/main.lua"
    LUA_PACKAGE_PATH="${CMAKE_SOURCE_DIR}/scripts/?.lua"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
)

add_executable(update update.cpp)
target_link_libraries(update
    liblua
    lfs
    luasocket
)
target_compile_definitions(update PRIVATE
    LUA_MAIN_SCRIPT="${CMAKE_CURRENT_SOURCE_DIR}/main.lua"
    LUA_PACKAGE_PATH="${CMAKE_SOURCE_DIR}/scripts/?.lua"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
)
