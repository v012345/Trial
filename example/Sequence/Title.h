#ifndef INCLUDED_SEQUENCE_TITLE_H
#define INCLUDED_SEQUENCE_TITLE_H

#include "Sequence/Child.h"

class Image;

namespace Sequence {
    class Parent;

    class Title : public Child {
      public:
        Title();
        ~Title();
        void update(Parent*);

      private:
        Image* mImage; // 主题画面
        int mCursorPosition;
    };

} // namespace Sequence

#endif