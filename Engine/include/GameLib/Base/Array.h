#ifndef INCLUDED_GAMELIB_ARRAY_H
#define INCLUDED_GAMELIB_ARRAY_H

namespace GameLib {

/// 防止忘记释放而开发的能够自动释放内存的数组
template <class T> class Array {
  public:
    /// 默认。尺寸是0。
    Array();
    /// 初始大小。所有元素都用默认构造器初始化。
    explicit Array(int size);
    // 初始化尺寸
    explicit Array(int size, const T&);
    ~Array();
    /// 用于后续调整空间。可以缩小，但不能扩大。
    void setSize(int size);
    /// 获取大小
    int size() const;
    /// 全部释放
    void clear();
    /// 下标索引（const）
    const T& operator[](int i) const;
    /// 下标索引（非const）
    T& operator[](int i);

  private:
    void operator=(const Array&); // 禁止赋值
    Array(const Array&); // 禁止复制构造函数

    T* mElements;
    int mSize;
};

} // namespace GameLib
#include "GameLib/Base/Impl/ArrayImpl.h" //里面是这个

#endif
