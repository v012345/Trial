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
    // 设置加法
    void setAdd(const Vector2&, const Vector2&);
    // 设置减法
    void setSub(const Vector2&, const Vector2&);
    // 设置乘法
    void setMul(const Vector2&, const Vector2&);
    // 设置插值（this = a + u * ab + v * ac）
    void setInterporation(const Vector2& a, const Vector2& ab, const Vector2& ac, double u, double v);

    double x;
    double y;
};

#endif
