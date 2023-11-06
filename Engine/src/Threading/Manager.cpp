#include "GameLib/GameLib.h"
//
#include "GameLib/Threading/Manager.h"
#include "GameLib/WindowCreator/WindowCreator.h"

#include "Threading/ManagerImpl.h"

namespace GameLib {
    namespace Threading {

        ManagerImpl* gManagerImpl = 0; // Impl实体

        Manager::Manager() {
            // 不允许来自其他线程的调用
            ASSERT(WindowCreator::isMainThread() && "you must call from MAIN thread");
        }

        void Manager::create(int additionalThreadNumber) {
            STRONG_ASSERT(!gManagerImpl);
            gManagerImpl = NEW ManagerImpl(additionalThreadNumber);
        }

        int Manager::getCoreNumber() const { return gManagerImpl->getCoreNumber(); }

        void Manager::destroy() { SAFE_DELETE(gManagerImpl); }

        Manager Manager::instance() { return Manager(); }

    } // namespace Threading
} // namespace GameLib
