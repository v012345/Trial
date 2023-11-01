add_executable(MyWork WIN32
    "Main.cpp"
    "Image.cpp"
    "Framework.cpp"
    "Config.cpp"
    "StringRenderer.cpp"
    "${CMAKE_SOURCE_DIR}/Engine/src/WindowCreator/WindowCreator.cpp"
)
target_include_directories(MyWork PRIVATE
    "${CMAKE_SOURCE_DIR}/Engine"
    "${CMAKE_SOURCE_DIR}/Engine/src"
    "${CMAKE_SOURCE_DIR}/Engine/include"
    "${CMAKE_SOURCE_DIR}/lua/src"
    "${CMAKE_SOURCE_DIR}/libs"
)
target_compile_definitions(MyWork PRIVATE

    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
    # LUA_MAIN_SCRIPT="${CMAKE_SOURCE_DIR}/scripts/Main.lua"
    CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
    LUA_PACKAGE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/scripts/?.lua"
)

target_link_libraries(MyWork
    GameLib
    liblua
    lfs
    png_shared
    freetype
)
