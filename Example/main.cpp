#include "GameLib/Framework.h"
#include "GameLib/GameLib.h"
#include "GameLib/Math.h"
#include "Image.h"
#include "Matrix22.h"
#include "Vector2.h"
#include <sstream>
using namespace std;

int myround(double a) {
    a += (a > 0.0) ? 0.5 : -0.5f;
    return static_cast<int>(a);
}

void transform(Vector2* out, const Vector2& in, const Vector2& scalingOffset, const Vector2& scalingRatio, const Vector2& rotationOffset, const Matrix22& rotationMatrix) {
    // 缩放比例
    out->setMul(scalingRatio, in);
    // 移动
    *out += scalingOffset;
    // 旋转中心偏移
    *out -= rotationOffset;
    // 旋转角度
    rotationMatrix.multiply(out, *out);
    // 旋转中心向后移
    *out += rotationOffset;
}

bool gFirstFrame = true;
Image* gImage;
int gCount;

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
        double rotation = static_cast<double>(gCount); // 用于放大率
        // 放大倍数
        Vector2 scalingRatio(1.1 + sin(rotation), 1.1 + cos(rotation));
        //
        Vector2 scalingOffset(ww / 2 - iw / 2 * scalingRatio.x, wh / 2 - ih / 2 * scalingRatio.y);
        // 旋转偏移
        Vector2 rotationOffset(ww / 2, wh / 2);
        // 旋转矩阵
        double sine = sin(rotation);
        double cosine = cos(rotation);
        Matrix22 rotationMatrix(cosine, -sine, sine, cosine);

        // 3打点
        Vector2 a, b, c;
        transform(&a, Vector2(0, 0), scalingOffset, scalingRatio, rotationOffset, rotationMatrix); // 左上方
        transform(&b, Vector2(iw, 0), scalingOffset, scalingRatio, rotationOffset, rotationMatrix); // 右上方
        transform(&c, Vector2(0, ih), scalingOffset, scalingRatio, rotationOffset, rotationMatrix); // 左下
        // 计算b-a,c-a
        Vector2 ab, ac;
        ab.setSub(b, a);
        ac.setSub(c, a);
        // 开始插值
        double rcpWidth = 1.0 / static_cast<double>(iw);
        double rcpHeight = 1.0 / static_cast<double>(ih);
        for (int y = 0; y < ih; ++y) {
            double yf = static_cast<double>(y) + 0.5;
            double v = yf * rcpHeight;
            for (int x = 0; x < iw; ++x) {
                double xf = static_cast<double>(x) + 0.5;
                double u = xf * rcpWidth;
                Vector2 p;
                p.setInterporation(a, ab, ac, u, v);
                p -= Vector2(0.5, 0.5); // 下标
                int rx, ry;
                rx = myround(p.x);
                ry = myround(p.y);
                // 如果在范围内则粘贴
                if (rx >= 0 && rx < ww && ry >= 0 && ry < wh) { vram[ry * ww + rx] = gImage->pixel(x, y); }
            }
        }
        ++gCount;

        ostringstream oss;
        oss << frameRate();
        drawDebugString(0, 0, oss.str().c_str());
    }
} // namespace GameLib