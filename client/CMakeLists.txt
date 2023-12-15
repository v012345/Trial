cmake_minimum_required(VERSION 3.20)

add_executable(client client.cpp)

target_include_directories(client PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/glfw/include
    ${CMAKE_SOURCE_DIR}/libs/glew-2.2.0/include
    ${CMAKE_SOURCE_DIR}/libs/imgui
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends
)
target_link_libraries(client
    liblua
    lfs
    luasocket
    luasec
    glfw
    glew
    imgui
)
target_compile_definitions(client PRIVATE
    LUA_MAIN_SCRIPT="${CMAKE_CURRENT_SOURCE_DIR}/main.lua"
    LUA_PACKAGE_PATH="${CMAKE_SOURCE_DIR}/scripts/?.lua"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
)