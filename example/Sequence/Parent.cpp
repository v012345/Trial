#include "Sequence/Parent.h"
#include "GameLib/GameLib.h"
#include "Sequence/Title.h"

namespace Sequence {

    Parent* Parent::mInstance = 0;

    void Parent::create() {
        ASSERT(!mInstance);
        mInstance = new Parent();
    }

    void Parent::destroy() {
        ASSERT(mInstance);
        SAFE_DELETE(mInstance);
    }

    Parent* Parent::instance() { //
        return mInstance;
    }

    Parent::Parent() : mMode(MODE_NONE), mChild(0) { //
        mChild = new Title();
    }

    Parent::~Parent() {
        // 如果剩余则销毁
        SAFE_DELETE(mChild);
    }

    void Parent::update() {
        Base* nextChild = mChild->update(this);
        if (nextChild != mChild) {
            // 迁移判断
            Child* casted = dynamic_cast<Child*>(nextChild);
            ASSERT(casted); //
            SAFE_DELETE(mChild);
            mChild = casted;
        }
        nextChild = 0; // 以防万一
    }

    void Parent::setMode(Mode mode) { //
        mMode = mode;
    }

    Parent::Mode Parent::mode() const { //
        return mMode;
    }

} // namespace Sequence
