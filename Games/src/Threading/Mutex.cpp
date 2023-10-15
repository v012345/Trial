#include <windows.h>
#undef min
#undef max
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/GameLib.h"
#include "GameLib/Threading/Mutex.h"

namespace GameLib {
namespace Threading {

// 要检测多个解锁，多个锁等，内容为信号量。
// 在发行版中切换到CriticalSection可能很好。
class Mutex::Impl : public ReferenceType {
  public:
    Impl() : mHandle(0) { mHandle = CreateSemaphore(NULL, 1, 1, NULL); }
    ~Impl() { CloseHandle(mHandle); }
    void lock() { WaitForSingleObject(mHandle, INFINITE); }
    void unlock() {
        LONG prev;
        ReleaseSemaphore(mHandle, 1, &prev);
        STRONG_ASSERT(prev == 0 && "unlock() is called twice! Check lock-unlock pair.");
    }
    HANDLE mHandle;
};

Mutex Mutex::create() {
    Mutex r;
    r.mImpl = NEW Impl();
    return r;
}

void Mutex::lock() { mImpl->lock(); }

void Mutex::unlock() { mImpl->unlock(); }

Mutex::Mutex() : mImpl(0) {}

Mutex::Mutex(const Mutex& o) : mImpl(o.mImpl) {
    if (mImpl) { mImpl->refer(); }
}

Mutex::~Mutex() { release(); }

void Mutex::release() {
    if (mImpl) {
        mImpl->release();
        if (mImpl->referenceCount() == 0) { SAFE_DELETE(mImpl); }
    }
    mImpl = 0;
}

Mutex& Mutex::operator=(const Mutex& o) {
    release();
    mImpl = o.mImpl;
    if (mImpl) { mImpl->refer(); }
    return *this;
}

bool Mutex::operator==(const Mutex& o) const { return (mImpl == o.mImpl); }

bool Mutex::operator!=(const Mutex& o) const { return (mImpl != o.mImpl); }

Mutex::operator void*() const { return (mImpl == 0) ? 0 : reinterpret_cast<void*>(~0); }

} // namespace Threading
} // namespace GameLib
