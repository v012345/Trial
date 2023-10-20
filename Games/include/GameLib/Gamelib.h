#ifndef INCLUDED_GAMELIB_GAMELIB_H
#define INCLUDED_GAMELIB_GAMELIB_H

namespace GameLib {
    enum Exception {
        EXCEPTION_IGNORE,
        EXCEPTION_EXIT,
    };
    void halt(const char* filename, int line, const char* message);
} // namespace GameLib

// 宏
#define STRONG_ASSERT(exp) ((!!(exp)) || (GameLib::halt(__FILE__, __LINE__, #exp), 0)) // 这行不好理解
#define HALT(exp)                                                                                                                                                                                      \
    { GameLib::halt(__FILE__, __LINE__, #exp); }
#define SAFE_DELETE(x)                                                                                                                                                                                 \
    {                                                                                                                                                                                                  \
        delete (x);                                                                                                                                                                                    \
        (x) = 0;                                                                                                                                                                                       \
    }
#define SAFE_DELETE_ARRAY(x)                                                                                                                                                                           \
    {                                                                                                                                                                                                  \
        delete[] (x);                                                                                                                                                                                  \
        (x) = 0;                                                                                                                                                                                       \
    }
#define OPERATOR_DELETE(x)                                                                                                                                                                             \
    {                                                                                                                                                                                                  \
        operator delete(x);                                                                                                                                                                            \
        (x) = 0;                                                                                                                                                                                       \
    }

// 判断调试和发布的分支
#ifndef NDEBUG
#define NEW new (__FILE__, __LINE__)
#define OPERATOR_NEW(x) operator new((x), __FILE__, __LINE__)
#define ASSERT(exp) ((!!(exp)) || (GameLib::halt(__FILE__, __LINE__, #exp), 0))
#else // NDEBUG
#define NEW new
#define OPERATOR_NEW(x) operator new(x)
#define ASSERT(exp)
#endif // NDEBUG

// 基本服务标头
#include "Base/Array.h"
#include "Base/DebugStream.h"
//  #include "Base/AutoPtr.h"
//  #include "Base/SharedPtr.h"
#include "Base/MemoryManager.h"
#include <new>

#endif
