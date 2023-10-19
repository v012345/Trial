add_executable(ConvertToCode ConvertToCode.cpp)
target_compile_definitions(ConvertToCode PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}/"
)