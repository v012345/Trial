// 这是供类编写者使用的
// 引用计数类型类始终具有一个功能，
// 使其更容易编写。
// #define TYPE ActualType
// 包含它。ActualType是类型的名称。

TYPE::TYPE() : mImpl(0) {}

TYPE::TYPE(const TYPE& o) : mImpl(o.mImpl) {
    if (mImpl) { mImpl->refer(); }
}

TYPE::~TYPE() { release(); }

void TYPE::release() {
    if (mImpl) {
        mImpl->release();
        if (mImpl->referenceCount() == 0) { SAFE_DELETE(mImpl); }
    }
    mImpl = 0;
}

TYPE& TYPE::operator=(const TYPE& o) {
    release();
    mImpl = o.mImpl;
    if (mImpl) { mImpl->refer(); }
    return *this;
}

bool TYPE::operator==(const TYPE& o) const { return (mImpl == o.mImpl); }

bool TYPE::operator!=(const TYPE& o) const { return (mImpl != o.mImpl); }

TYPE::operator void*() const { return (mImpl == 0) ? 0 : reinterpret_cast<void*>(~0); }

#undef TYPE
