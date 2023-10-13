cmake_minimum_required(VERSION 3.20)
add_executable(NimotsuKunRawMemory FirstGame/NimotsuKunRawMemory/main.cpp)
add_executable(NimotsuKun FirstGame/NimotsuKun/main.cpp)
add_executable(NimotsuKun2 FirstGame/NimotsuKun2/main.cpp)
target_compile_definitions(NimotsuKun2 PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)