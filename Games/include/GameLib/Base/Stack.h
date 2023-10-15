#ifndef INCLUDED_GAMELIB_STACK_H
#define INCLUDED_GAMELIB_STACK_H

namespace GameLib {

/// 简易的容量固定堆栈
template <class T> class Stack {
  public:
    /// 默认。大小，最大容量为0。
    Stack();
    /// 指定最大容量。
    explicit Stack(int capacity);
    ~Stack();
    /// 再次确保。在有内容的状态下调用将触发assert。执行clear()
    void setCapacity(int size);
    /// 获取最大容量
    int capacity() const;
    /// 获得当前元素数
    int size() const;
    /// 全部释放
    void clear();
    /// 添加到末尾
    void push(const T&);
    /// 添加到末尾（调用默认构造器）
    void push();
    /// 删除结尾
    void pop();
    /// 删除结尾（接收要丢弃的元素的副本）
    void pop(T* out);
    /// 获取尾部（const）
    const T* get() const;
    /// 获取尾部（非const）
    T* get();

  private:
    void operator=(const Stack&); // 禁止赋值
    Stack(const Stack&); // 禁止复制构造函数

    T* mElements;
    int mSize;
    int mCapacity;
    int mBack;
};

} // namespace GameLib
#include "GameLib/Base/Impl/StackImpl.h" //里面是这个

#endif
