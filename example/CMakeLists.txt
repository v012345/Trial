
file(GLOB_RECURSE SRCS *.cpp)
add_executable(Example WIN32
    ${SRCS}
    # "../src/WindowCreator/WindowCreator.cpp"
)
target_include_directories(Example PRIVATE
    "../include"
    "../src"
)

target_compile_definitions(Example PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
    CMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)

target_link_libraries(Example
    # GameLib
)