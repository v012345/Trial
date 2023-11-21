#include "GameLib/Framework.h"
#include "GameLib/GameLib.h"
#include <sstream>
using namespace GameLib;

#include "GameLib/Math.h"
#include "Image.h"
#include "Matrix22.h"
#include "Vector2.h"
using namespace std;

void rotate(Vector2* out, const Vector2& in, const Vector2& offset, const Matrix22& matrix) {
    Vector2 tmpOffset(-0.5, -0.5);
    tmpOffset += offset;
    // 移动原点
    out->setSub(in, tmpOffset);
    // 矩阵相乘
    matrix.multiply(out, *out);
    // 恢复原点并转换为下标
    *out += tmpOffset;
}

bool gFirstFrame = true;
Image* gImage;
int gCount;

// 栅格化三角形
void rasterize(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& ai, const Vector2& bi, const Vector2& ci, const Image* image) {
    // 使用abc以升序排列y。
    Vector2 p0, p1, p2;
    Vector2 i0, i1, i2;
    // 以左上角为原点, y 向下, x 向右 的系来看的话
    if (a.y < b.y) { // a 在 b 上方
        // b 在 c 上方
        if (b.y < c.y) { // abc
            p0 = a;
            p1 = b;
            p2 = c;
            i0 = ai;
            i1 = bi;
            i2 = ci;
        } else if (a.y < c.y) { // acb
            p0 = a;
            p1 = c;
            p2 = b;
            i0 = ai;
            i1 = ci;
            i2 = bi;
        } else { // cab
            p0 = c;
            p1 = a;
            p2 = b;
            i0 = ci;
            i1 = ai;
            i2 = bi;
        }
    } else {
        if (a.y < c.y) { // bac
            p0 = b;
            p1 = a;
            p2 = c;
            i0 = bi;
            i1 = ai;
            i2 = ci;
        } else if (b.y < c.y) { // bca
            p0 = b;
            p1 = c;
            p2 = a;
            i0 = bi;
            i1 = ci;
            i2 = ai;
        } else { // cba
            p0 = c;
            p1 = b;
            p2 = a;
            i0 = ci;
            i1 = bi;
            i2 = ai;
        }
    }

    Vector2 p01, p02, p03;
    p01.setSub(p1, p0);
    p02.setSub(p2, p0);

    Vector2 p3; // 就是过 p1 与 x 轴平行的直线与 p0p2 的交点
    double t = (p1.y - p0.y) / p02.y; // t作为临时变量使用
    p3.x = p0.x + p02.x * t;
    p3.y = p1.y;

    p03.setSub(p3, p0);

    // 获取VRAM
    unsigned* vram = Framework::instance().videoMemory();
    int ww = Framework::instance().width();

    // 图像侧的下标与屏幕上的位置之间的关系由等式表示。
    // 要求出x和y，就必须知道ix和iy。

    double a0, a1, a2, a3, a4, a5; // 代替abcdef
    Vector2 i01, i02;
    i01.setSub(i1, i0);
    i02.setSub(i2, i0);
    // 一个映射用的矩阵, 从可视三角形中上点, 映射回纹理三角形上
    // 即 P = AI, 因为两个三角形为已知量, 使用待定系数法, 可以解出下面 6 个值
    a0 = (i01.x * p02.y - i02.x * p01.y) / (p01.x * p02.y - p02.x * p01.y);
    a1 = (i01.x * p02.x - i02.x * p01.x) / (p01.y * p02.x - p02.y * p01.x);
    a2 = i0.x - p0.x * a0 - p0.y * a1;
    a3 = (i01.y * p02.y - i02.y * p01.y) / (p01.x * p02.y - p02.x * p01.y);
    a4 = (i01.y * p02.x - i02.y * p01.x) / (p01.y * p02.x - p02.y * p01.x);
    a5 = i0.y - p0.x * a3 - p0.y * a4;

    // 从上半三角形开始。p0，p1，p3。
    // 首先，将Y的范围设为整数。
    if (p1.y > p0.y + 1.0) {
        int yBegin = static_cast<int>(p0.y);
        int yEnd = static_cast<int>(p1.y);
        double rcpP01y = 1.0 / p01.y;
        for (int y = yBegin; y <= yEnd; ++y) {
            // 检查指定y处x的范围。
            double yf = y;
            // 只需找到p3-p0和p1-p0的两边与y的当前水平线的交点即可。
            // 计算与以前相同。
            t = (y - p0.y) * rcpP01y;
            double x0 = p0.x + p01.x * t; // p0.x + p01.x * ( y - p0.y ) / p01.y
            double x1 = p0.x + p03.x * t; // p0.x + p01.x *（y-p0.y）/ p03.y不过p01.y = p03.y
            int xBegin, xEnd;
            if (x0 > x1) {
                xBegin = static_cast<int>(x1);
                xEnd = static_cast<int>(x0);
            } else {
                xBegin = static_cast<int>(x0);
                xEnd = static_cast<int>(x1);
            }
            for (int x = xBegin; x <= xEnd; ++x) {
                double xf = x;
                // 从x，y中找到ix，iy
                Vector2 ip;
                ip.x = a0 * xf + a1 * yf + a2;
                ip.y = a3 * xf + a4 * yf + a5;
                // 获取像素
                vram[y * ww + x] = image->pixel(ip);
            }
        }
    }
    // 接下来是下半三角形。完成的工作几乎是相同的。
    if (p2.y > p1.y + 1.0) {
        Vector2 p21, p23;
        p21.setSub(p1, p2);
        p23.setSub(p3, p2);
        int yBegin = static_cast<int>(p1.y);
        int yEnd = static_cast<int>(p2.y);
        double rcpP21y = 1.0 / p21.y;
        for (int y = yBegin; y <= yEnd; ++y) {
            // 检查指定y处x的范围。
            double yf = y;
            // 只需找到p3-p2和p1-p2的两边与y的当前水平线的交点即可。
            // 计算与以前相同。
            t = (y - p2.y) * rcpP21y;
            double x0 = p2.x + p21.x * t; // p2.x + p21.x * ( y - p2.y ) / p21.y
            double x1 = p2.x + p23.x * t; // p2.x + p21.x *（y-p2.y）/ p23.y其中p21.y = p23.y.
            int xBegin, xEnd;
            if (x0 > x1) {
                xBegin = static_cast<int>(x1);
                xEnd = static_cast<int>(x0);
            } else {
                xBegin = static_cast<int>(x0);
                xEnd = static_cast<int>(x1);
            }
            for (int x = xBegin; x <= xEnd; ++x) {
                double xf = x;
                // 从x，y中找到ix，iy
                Vector2 ip;
                ip.x = a0 * xf + a1 * yf + a2;
                ip.y = a3 * xf + a4 * yf + a5;
                // 获取像素
                vram[y * ww + x] = image->pixel(ip);
            }
        }
    }
}

namespace GameLib {
    void Framework::update() {
        if (gFirstFrame) {
            gImage = new Image(CMAKE_CURRENT_SOURCE_DIR "background.dds");
            gFirstFrame = false;
        }
        unsigned* vram = videoMemory();
        int ww = width(); // window width
        int wh = height(); // window height
        // 全黑
        for (int i = 0; i < ww * wh; ++i) { vram[i] = 0; }
        int iw = gImage->width(); // image width
        int ih = gImage->height(); // image height
        // 偏移量在屏幕上居中。（160,120）
        Vector2 offset;
        offset.x = static_cast<double>(ww) / 2.0;
        offset.y = static_cast<double>(wh) / 2.0;
        double rotation = static_cast<double>(gCount);
        double sine = sin(rotation); //
        double cosine = cos(rotation);
        Matrix22 matrix(cosine, -sine, sine, cosine); // 创建矩阵
        // Matrix22 matrix(sine + 0.5, 0, 0, sine + 0.5); // 创建矩阵
        // 4打点
        Vector2 a, b, c, d;
        rotate(&a, Vector2((ww - iw) / 2, (wh - ih) / 2), offset, matrix); // 左上方
        rotate(&b, Vector2((ww + iw) / 2 - 1, (wh - ih) / 2), offset, matrix); // 右上方
        rotate(&c, Vector2((ww - iw) / 2, (wh + ih) / 2 - 1), offset, matrix); // 左下
        // 4第二个点可以用a+（b-a）+（c-a）计算。
        Vector2 ab, ac;
        ab.setSub(b, a);
        ac.setSub(c, a);
        d.setAdd(a, ab);
        d += ac;
        // 计算a，b，c，d中图像侧的下标值
        Vector2 ai(0, 0);
        Vector2 bi(iw - 1, 0); //(w-1)/w, 0)
        Vector2 ci(0, ih - 1); //(0,(h-1)/h)
        Vector2 di(bi.x, ci.y);
        // 开始栅格化
        rasterize(a, b, c, ai, bi, ci, gImage); // 三角形abc
        rasterize(d, b, c, di, bi, ci, gImage); // 三角形dbc
        ++gCount;

        ostringstream oss;
        oss << frameRate();
        drawDebugString(0, 0, oss.str().c_str());
    }
} // namespace GameLib