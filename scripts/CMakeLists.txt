add_executable(MyWork WIN32
    "Main.cpp"
    "../Engine/src/WindowCreator/WindowCreator.cpp"
)
target_include_directories(MyWork PRIVATE
    "../Engine/src"
    "../Engine/include"
)
target_compile_definitions(MyWork PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
    LUA_MAIN_SCRIPT="${CMAKE_SOURCE_DIR}/scripts/Main.lua"
    LUA_PACKAGE_PATH="${CMAKE_SOURCE_DIR}/scripts/?.lua"
)

target_link_libraries(MyWork
    GameLib
    liblua
    lfs
    png_shared
    freetype
)
