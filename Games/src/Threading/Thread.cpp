#include "GameLib/GameLib.h"
//
#include "GameLib/Threading/Event.h"
#include "GameLib/Threading/Thread.h"
#include "Threading/ManagerImpl.h"

namespace GameLib {
namespace Threading {

class Thread::Impl {
  public:
    Impl() { mFinished = Event::create(); }
    ~Impl() {}
    Event mFinished;
};

Thread::Thread() : mImpl(0) {}

void Thread::start() {
    wait(); // 如果前面有则等待
    mImpl = new Impl;
    gManagerImpl->add(this, &mImpl->mFinished); // 开始运行
}

void Thread::wait() {
    if (mImpl) {
        mImpl->mFinished.wait();
        SAFE_DELETE(mImpl);
    }
}

bool Thread::isFinished() {
    bool r = false;
    if (mImpl) {
        if (mImpl->mFinished.isSet()) { // 完成
            SAFE_DELETE(mImpl);
            r = true;
        }
    } else {
        r = true;
    }
    return r;
}

Thread::~Thread() { STRONG_ASSERT(!mImpl && "You must call wait() or isFinished() before destruction."); }

} // namespace Threading
} // namespace GameLib
