#ifndef INCLUDED_VECTOR2_H
#define INCLUDED_VECTOR2_H

class Vector2 {
  public:
    // 一个什么都不做的构造函数
    Vector2();
    // 通过整数构造函数
    Vector2(int x, int y);
    // 根据double构造
    Vector2(double x, double y);
    // 通过Vector2构造
    Vector2(const Vector2&);
    // 加法
    void operator+=(const Vector2&);
    // 减法
    void operator-=(const Vector2&);
    // 复制
    void operator=(const Vector2&);

    double x;
    double y;
};

#endif
