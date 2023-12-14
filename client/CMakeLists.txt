cmake_minimum_required(VERSION 3.20)

add_executable(client client.cpp)
target_link_libraries(client
    liblua
    lfs
    luasocket
    luasec
)
target_compile_definitions(client PRIVATE
    LUA_MAIN_SCRIPT="${CMAKE_CURRENT_SOURCE_DIR}/main.lua"
    LUA_PACKAGE_PATH="${CMAKE_SOURCE_DIR}/scripts/?.lua"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
)