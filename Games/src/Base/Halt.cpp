#include "GameLib/GameLib.h"
#include <cstdio>
#include <windows.h>

bool gThrowWhenHalt = false;

// 注意不要在此文件中new。
// 因为它可以从new中调用。
namespace GameLib {

void halt(const char* filename, int line, const char* message) {
    char buf[8192];
    sprintf_s(buf, 8192, "%s:%d : %s", filename, line, message);
    if (gThrowWhenHalt) {
        ::OutputDebugStringA(buf);
        ::OutputDebugStringA("\n");
        throw EXCEPTION_IGNORE; //
    } else {
        // 弹出消息框
        MessageBoxA(0, buf, "Critical Error!", MB_OK | MB_ICONERROR);
        // 马上结束
        *reinterpret_cast<int*>(0) = 0;
    }
}

} // namespace GameLib
