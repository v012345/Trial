cmake_minimum_required(VERSION 3.20)
project(Trial)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory("lua-5.4.6/src")
add_subdirectory("lua-extensions")
add_subdirectory("Games")
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}
    liblua-5.4.6
    lfs
)
target_include_directories(${PROJECT_NAME} PRIVATE
    lua-5.4.6/src

    # lua-extensions
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    LUA_MAIN_SCRIPT="${CMAKE_SOURCE_DIR}/scripts/main.lua"
    LUA_PACKAGE_PATH="${CMAKE_SOURCE_DIR}/scripts/?.lua"
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)