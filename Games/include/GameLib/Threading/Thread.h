#ifndef INCLUDED_GAMELIB_THREADING_THREAD_H
#define INCLUDED_GAMELIB_THREADING_THREAD_H

namespace GameLib {
namespace Threading {

/// 线程抽象基类

class Thread {
  public:
    /// 自己做
    virtual void operator()() = 0;
    /// 开始执行。如果正在执行，将等待结束再执行。
    void start();
    /// 等待结束
    void wait();
    /// 问结束了没有
    bool isFinished();
    /// 无法直接调用构造函数
    Thread();

  protected: // 不能直接delete基类。删除派生类。
    ~Thread();

  private:
    class Impl;
    Impl* mImpl;
};

} // namespace Threading
} // namespace GameLib

#endif
