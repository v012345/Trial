#include "include/GameLib/Base/Conversion.h"
#include "include/GameLib/Base/IBinaryStream.h"
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
    // std::cout << ok << std::endl;
    return 0;
}
