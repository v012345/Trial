#ifndef INCLUDED_ARRAY_2D_H
#define INCLUDED_ARRAY_2D_H

// 2D数组类
// 模板熟悉吗？如果不熟悉，请学习基础知识。
// 此类声明中将其视为存在T类，
// 使用它时，将int或bool代替T并使用它。
template <class T> class Array2D {
  public:
    Array2D() : mArray(0) {}
    ~Array2D() {
        delete[] mArray;
        mArray = 0; // 将指针赋值为0是一种习惯。
    }
    void setSize(int size0, int size1) {
        if (mArray) {
            delete[] mArray;
            mArray = 0;
        }
        mSize0 = size0;
        mSize1 = size1;
        mArray = new T[size0 * size1];
    }
    T& operator()(int index0, int index1) { return mArray[index1 * mSize0 + index0]; }
    const T& operator()(int index0, int index1) const { return mArray[index1 * mSize0 + index0]; }

  private:
    T* mArray;
    int mSize0;
    int mSize1;
};

#endif
