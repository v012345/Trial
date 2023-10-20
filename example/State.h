#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "Array2D.h"

class Image; // 只需要声明。不必包含它。

// 状态类
class State {
  public:
    State(const char* stageData, int size);
    ~State();
    void update(int moveX, int moveY);
    void draw() const;
    bool hasCleared() const;
    void reset(); // 返回初始状态
    static const int MAX_MOVE_COUNT = 15;

  private:
    class Object;
    void setSize();

    int mWidth;
    int mHeight;
    Array2D<Object> mObjects;
    Image* mImage; // 图片
    int mMoveCount;
    char* mStageData;
    int mStageDataSize;
};

#endif
