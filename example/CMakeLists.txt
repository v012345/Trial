
file(GLOB_RECURSE SRCS *.cpp)
add_executable(Example
    WIN32
    ${SRCS}
    "${CMAKE_SOURCE_DIR}/Engine/src/WindowCreator/WindowCreator.cpp"
)

target_include_directories(Example PRIVATE
    "${CMAKE_SOURCE_DIR}/Engine"
    "${CMAKE_SOURCE_DIR}/Engine/src"
    "${CMAKE_SOURCE_DIR}/Engine/include"
    "${CMAKE_SOURCE_DIR}/Example"
)
target_compile_definitions(Example PRIVATE
    CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"

    # CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
)

target_link_libraries(Example
    GameLib
)