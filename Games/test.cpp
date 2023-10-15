#include "include/GameLib/Base/Conversion.h"
#include <iostream>
int main(int argc, char const* argv[]) {
    /* code */
    char out[1024];
    int i = GameLib::toString16(out, 10);
    out[i] = '\0';
    std::cout << out << std::endl;
    return 0;
}
