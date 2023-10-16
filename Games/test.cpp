// #include "Framework.h"
// //
// #include "include/GameLib/Base/Compressor.h"
// #include "include/GameLib/Base/Conversion.h"
// #include "include/GameLib/Base/DebugStream.h"
// #include "include/GameLib/Base/IBinaryStream.h"
#include "include/GameLib/Base/MemoryManager.h"
// #include "include/GameLib/Base/PrimeNumber.h"
// #include "include/GameLib/FileIO/Manager.h"
// #include "include/GameLib/Input/Manager.h"
// #include <iostream>

// namespace GameLib {
// void Framework::update() {
//     unsigned* vram = videoMemory();
//     static unsigned i;
//     vram[i] += i * 100;
//     i += 9973; // 小于1万的最大的质数
//     i %= (width() * height());
// }
// } // namespace GameLib

int main(int argc, char const* argv[]) {
    GameLib::MemoryManager::instance();
    return 0;
}
