#ifndef INCLUDED_GAMELIB_TANK_H
#define INCLUDED_GAMELIB_TANK_H

#include "GameLib/Base/Array.h"

namespace GameLib {

/// 在添加完之前无法访问，要添加多少并不清楚

template <class T> class Tank {
  public:
    explicit Tank(int blockSize = 16);
    ~Tank();
    /// 再次确保。在有内容的状态下调用将触发assert。执行clear()
    void setBlockSize(int blockSize);
    /// 获得当前元素数
    int size() const;
    /// 全部释放
    void clear();
    /// 把空添加到结尾处。因为要返回新增元素的指针，所以初期设定使用这个
    T* add();
    /// 添加到末尾
    void add(const T&);
    /// 下一个
    void toNext();
    /// 结束了
    bool isEnd() const;
    /// 获取当前元素（非const）
    T* get();
    /// 获取当前元素（const）
    const T* get() const;
    /// 用指针返回第一个元素
    void rewind();
    /// 转换成简单的数组。根据size()的尺寸申请的空间
    void copyTo(T*) const;
    // 复制到Arry。不允许为空
    void copyTo(Array<T>*) const;

  private:
    struct Block {
        Block* mNext; // 下一个节点
        T* mElements; // 数组
    };
    void operator=(const Tank&); // 禁止赋值
    Tank(const Tank&); // 禁止复制构造函数

    // 常量
    int mBlockSize; // 单位量
    int mSize;
    Block mHead; // 虚拟的

    Block* mLastBlock; // 最后一个节点
    int mLastPos; // 在最后一个节点内的位置
    Block* mCurrentBlock; // 当前的迭代器
    int mCurrentPos; // 节点位置
};

} // namespace GameLib
#include "GameLib/Base/Impl/TankImpl.h" //里面是这个

#endif
