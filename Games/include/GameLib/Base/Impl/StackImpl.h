#ifndef INCLUDED_GAMELIB_STACKIMPL_H
#define INCLUDED_GAMELIB_STACKIMPL_H

#include "GameLib/GameLib.h"
//
#include "GameLib/Base/Stack.h"

namespace GameLib {

template <class T> inline Stack<T>::Stack() : mElements(0), mSize(0), mCapacity(0), mBack(0) {}

template <class T> inline Stack<T>::Stack(int capacity) : mElements(0), mSize(0), mCapacity(0), mBack(0) {
    if (capacity <= 0) { return; }
    mCapacity = capacity;
    // 因为不想调用构造函数所以new
    mElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
}

template <class T> inline Stack<T>::~Stack() {
    if (mElements) {
        clear();
        OPERATOR_DELETE(mElements);
    }
}

template <class T> inline void Stack<T>::setCapacity(int capacity) {
    ASSERT(mSize == 0 && "NOT EMPTY!. you must call clear().");
    // 释放
    if (mElements) {
        clear();
        OPERATOR_DELETE(mElements);
    }
    mCapacity = mBack = 0;
    if (capacity <= 0) { return; }
    mCapacity = capacity;
    // 重新分配
    // 因为不想调用构造函数所以new
    mElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
}

template <class T> inline int Stack<T>::capacity() const { return mCapacity; }

template <class T> inline int Stack<T>::size() const { return mSize; }

template <class T> inline void Stack<T>::clear() {
    for (int i = 0; i < mBack; ++i) {
        mElements[i].~T(); // 明确销毁
    }
    mBack = mSize = 0;
}

template <class T> inline void Stack<T>::push(const T& v) {
    ASSERT(mSize < mCapacity);
    new (&mElements[mBack]) T(v); // 调用复制构造函数
    ++mBack;
    ++mSize;
}

template <class T> inline void Stack<T>::push() {
    ASSERT(mSize < mCapacity);
    new (&mElements[mBack]) T; // 默认调用构造函数
    ++mBack;
    ++mSize;
}

template <class T> inline void Stack<T>::pop() {
    ASSERT(mSize > 0);
    --mBack;
    mElements[mBack].~T(); // 明确销毁
    --mSize;
}

template <class T> inline void Stack<T>::pop(T* o) {
    ASSERT(mSize > 0);
    *o = mElements[mBack - 1];
    --mBack;
    mElements[mBack].~T(); // 明确销毁
    --mSize;
}

template <class T> inline const T* Stack<T>::get() const {
    ASSERT(mSize > 0);
    return &mElements[mBack - 1];
}

template <class T> inline T* Stack<T>::get() {
    ASSERT(mSize > 0);
    return &mElements[mBack - 1];
}

} // namespace GameLib

#endif
