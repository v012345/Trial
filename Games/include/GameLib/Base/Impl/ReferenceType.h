#ifndef INCLUDED_GAMELIB_BASE_REFERENCETYPE_H
#define INCLUDED_GAMELIB_BASE_REFERENCETYPE_H

// 基类可简化使用引用计数
namespace GameLib {

class ReferenceType {
  public:
    ReferenceType() : mReferenceCount(1) {}
    void refer() { ++mReferenceCount; }
    void release() { --mReferenceCount; }
    int referenceCount() const { return mReferenceCount; }

  protected:
    ~ReferenceType() {}

  private:
    int mReferenceCount;
};

} // namespace GameLib

#endif
