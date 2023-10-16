#ifndef INCLUDED_GAMELIB_THREADING_MANAGERIMPL_H
#define INCLUDED_GAMELIB_THREADING_MANAGERIMPL_H

#include <process.h>
#include <windows.h>
#undef min
#undef max
#include <list>

#include "GameLib/GameLib.h"
//
#include "GameLib/Threading/Event.h"
#include "GameLib/Threading/Manager.h"
#include "GameLib/Threading/Mutex.h"
#include "GameLib/Threading/Semaphore.h"
#include "GameLib/Threading/Thread.h"

namespace GameLib {
namespace Threading {
using namespace std;

class ManagerImpl {
  public:
    struct Entry {
        Entry() : mThread(0), mFinished(0) {}
        Entry(Thread* t, Event* e) : mThread(t), mFinished(e) {}
        ~Entry() {
            mThread = 0;
            mFinished = 0;
        }
        Thread* mThread;
        Event* mFinished;
    };
    ManagerImpl(int additionalThreadNumber) : mThreads(0), mThreadNumber(0), mEndRequest(false), mCoreNumber(1) { // 至少1
        mQueueSemaphore = Semaphore::create(0, 0x7fffffff); // 最大值是无限的
        mLock = Mutex::create();

        // 获取核数
#ifdef NDEBUG // 在Debug版本中，只会自动创建一个。为了防止在开发机器上运行而不能在其他机器上运行。
        HANDLE process = GetCurrentProcess();
        DWORD processMask;
        DWORD systemMask;
        BOOL succeeded = GetProcessAffinityMask(process, &processMask, &systemMask);
        if (succeeded != 0) { //
            for (int i = 1; i < 32; ++i) {
                if (processMask & (1 << i)) { ++mCoreNumber; }
            }
        }
#endif
        mThreadNumber = additionalThreadNumber + mCoreNumber + 1; // 至少使内核数+ 1（加载器使用）
        mThreads = new HANDLE[mThreadNumber];
        // 开始
        for (int i = 0; i < mThreadNumber; ++i) {
            unsigned dummy;
            mThreads[i] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, execute, this, 0, &dummy));
            SetThreadPriority(mThreads[i], THREAD_PRIORITY_BELOW_NORMAL); // 优先级稍低
        }
    }
    ~ManagerImpl() {
        // 等待完成
        mLock.lock();
        mEndRequest = true;
        mLock.unlock();

        // 等待线程终止
        for (int i = 0; i < mThreadNumber; ++i) {
            while (true) {
                //
                mQueueSemaphore.increase(); // 队列中有东西
                if (WAIT_OBJECT_0 == WaitForSingleObject(mThreads[i], 1)) {
                    CloseHandle(mThreads[i]);
                    mThreads[i] = 0;
                    break;
                }
            }
        }
        SAFE_DELETE_ARRAY(mThreads);
    }
    static unsigned WINAPI execute(void* p) {
        ManagerImpl* m = reinterpret_cast<ManagerImpl*>(p);
        m->execute();
        return 0;
    }
    void execute() {
        while (true) {
            mQueueSemaphore.decrease(); // 等待队列添加

            mLock.lock();
            if (mEndRequest) {
                mLock.unlock();
                break;
            }
            Entry e = mQueue.front();
            mQueue.pop_front(); // 出列
            mLock.unlock();

            (*(e.mThread))(); // 运行
            e.mFinished->set(); // 标记结束
        }
    }
    void add(Thread* t, Event* finishFlag) {
        Entry e(t, finishFlag);

        mLock.lock();
        mQueue.push_back(e);
        mLock.unlock();

        mQueueSemaphore.increase(); // 已加
    }
    int getCoreNumber() const { return mCoreNumber; }

  private:
    HANDLE* mThreads; // 线程句柄数组
    int mThreadNumber;
    bool mEndRequest;
    Semaphore mQueueSemaphore;
    Mutex mLock;
    list<Entry> mQueue; // 请求队列
    int mCoreNumber;
};
extern ManagerImpl* gManagerImpl;

} // namespace Threading
} // namespace GameLib

#endif
