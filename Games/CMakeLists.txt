cmake_minimum_required(VERSION 3.20)
add_executable(NimotsuKunRawMemory FirstGame/NimotsuKunRawMemory/main.cpp)
add_executable(NimotsuKun FirstGame/NimotsuKun/main.cpp)
add_executable(NimotsuKun2 FirstGame/NimotsuKun2/main.cpp)
target_compile_definitions(NimotsuKun2 PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)
include_directories(include)
add_library(FileIOManager STATIC
    "src/Base/Conversion.cpp"
    "src/Base/IBinaryStream.cpp"
    "src/Base/RefString.cpp"
    "src/Base/OStringStream.cpp"
    "src/Base/PrimeNumber.cpp"
    "src/Base/Compressor.cpp"
    "src/Base/DebugStream.cpp"
    "src/Base/Halt.cpp"
)
add_library(ThreadingManager STATIC
    "src/Threading/Mutex.cpp"
)
add_library(GameLib STATIC
    "Framework.cpp"
)
add_executable(test test.cpp)
target_link_libraries(test
    FileIOManager
    ThreadingManager
)