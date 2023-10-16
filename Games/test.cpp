#include "include/GameLib/Base/Compressor.h"
#include "include/GameLib/Base/Conversion.h"
#include "include/GameLib/Base/DebugStream.h"
#include "include/GameLib/Base/IBinaryStream.h"
#include "include/GameLib/Base/MemoryManager.h"
#include "include/GameLib/Base/PrimeNumber.h"
#include "include/GameLib/FileIO/Manager.h"
#include <iostream>
int main(int argc, char const* argv[]) {
    /* code */
    char out[1024];
    int i = GameLib::toString16(out, 10);
    out[i] = '\0';
    std::cout << out << std::endl;
    char i1[] = "abc";
    GameLib::IBinaryStream ibs(i1);
    bool ok = true;
    // std::cout << ok << std::endl;
    char o1 = '\0';
    while (ok) {
        o1 = ibs.readChar(&ok);
        std::cout << o1 << std::endl;
    }

    std::cout << o1 << std::endl;
    std::cout << GameLib::PrimeNumber::isPrimeNumber(2) << std::endl;
    std::cout << GameLib::PrimeNumber::isPrimeNumber(4) << std::endl;
    char outData[1024];
    int outSize = 0;
    char data[100] = "asdf";
    // GameLib::Compressor::decompress(outData, &outSize, data, 4);
    // GameLib::DebugStream pDebugStream;
    // pDebugStream.begin();
    // pDebugStream << "jife";
    // GameLib::FileIO::Manager::instance().create();
    // GameLib::MemoryManager::instance().write();
    // pDebugStream.end();
    return 0;
}
