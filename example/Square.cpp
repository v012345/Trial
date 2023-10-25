#include "Square.h"

Square::Square() {}

Square::Square(int x, int y, int halfSize) : mX(x), mY(y), mHalfSize(halfSize) {}

void Square::set(int x, int y, int halfSize) {
    mX = x;
    mY = y;
    mHalfSize = halfSize;
}

bool Square::isIntersect(const Square& b) const {
    int al = mX - mHalfSize; // left A
    int ar = mX + mHalfSize; // right A
    int bl = b.mX - b.mHalfSize; // left B
    int br = b.mX + b.mHalfSize; // right B
    if ((al < br) && (ar > bl)) {
        int at = mY - mHalfSize; // top A
        int ab = mY + mHalfSize; // bottom A
        int bt = b.mY - b.mHalfSize; // top B
        int bb = b.mY + b.mHalfSize; // bottom B
        if ((at < bb) && (ab > bt)) { return true; }
    }
    return false;
}

void Square::getMoveLimit(int* dx, int* dy, const Square& wall, int iteration) const {
    int numerator = 1; // 分子
    int lastSucceededNumerator = 0; //
    int denominator = 1; // 分母
    Square tSquare; // 临时的
    for (int i = 0; i < iteration; ++i) {
        int tDx = *dx * numerator / denominator;
        int tDy = *dy * numerator / denominator;
        tSquare.set(mX + tDx, mY + tDy, mHalfSize);
        numerator *= 2;
        denominator *= 2;
        if (tSquare.isIntersect(wall)) {
            numerator -= 1;
        } else {
            lastSucceededNumerator = numerator; // 更新成功值
            numerator += 1;
        }
    }
    *dx = *dx * lastSucceededNumerator / denominator;
    *dy = *dy * lastSucceededNumerator / denominator;
}
