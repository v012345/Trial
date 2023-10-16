cmake_minimum_required(VERSION 3.20)
add_executable(NimotsuKunRawMemory FirstGame/NimotsuKunRawMemory/main.cpp)
add_executable(NimotsuKun FirstGame/NimotsuKun/main.cpp)
add_executable(NimotsuKun2 FirstGame/NimotsuKun2/main.cpp)
target_compile_definitions(NimotsuKun2 PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)
include_directories(include)
add_library(Base STATIC
    "src/Base/Conversion.cpp"
    "src/Base/IBinaryStream.cpp"
    "src/Base/RefString.cpp"
    "src/Base/OStringStream.cpp"
    "src/Base/PrimeNumber.cpp"
    "src/Base/Compressor.cpp"
    "src/Base/DebugStream.cpp"
    "src/Base/Halt.cpp"
    "src/Base/MemoryManager.cpp"
)
add_library(FileIOManager STATIC
    "src/FileIO/Manager.cpp"
    "src/FileIO/InFile.cpp"
    "src/FileIO/OutFile.cpp"
)

target_include_directories(FileIOManager PRIVATE src)
add_library(ThreadingManager STATIC
    "src/Threading/Manager.cpp"
    "src/Threading/Mutex.cpp"
    "src/Threading/Event.cpp"
    "src/Threading/Functions.cpp"
    "src/Threading/Thread.cpp"
    "src/Threading/Semaphore.cpp"
)
target_include_directories(ThreadingManager PRIVATE src)
add_library(GameLib STATIC
    "Framework.cpp"
)
add_executable(test test.cpp)
target_link_libraries(test
    Base
    FileIOManager
    ThreadingManager
)