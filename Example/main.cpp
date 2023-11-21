#include "GameLib/Framework.h"
#include "GameLib/GameLib.h"
#include "GameLib/Math.h"
#include "Image.h"
#include "Matrix23.h"
#include "Vector2.h"
#include <sstream>
using namespace std;

int myround(double a) {
    a += (a > 0.0) ? 0.5 : -0.5f;
    return static_cast<int>(a);
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
        // 创建矩阵
        double rotation = static_cast<double>(gCount);
        // 从图像的中心到原点，放大，旋转并返回中心。
        Matrix23 translationMatrix1;
        Matrix23 scalingMatrix;
        Matrix23 rotationMatrix;
        Matrix23 translationMatrix2;
        // 移动1（原点位于图像中心）
        translationMatrix1.setTranslation(Vector2(-iw / 2, -ih / 2));
        // 放大
        Vector2 scale(sin(rotation) * 2.0 + 1.0, cos(rotation) * 2.0 + 1.0);
        scalingMatrix.setScaling(Vector2(scale.x, scale.y));
        // 旋转角度
        rotationMatrix.setRotation(rotation);
        // 放回去
        translationMatrix2.setTranslation(Vector2(iw / 2, ih / 2));
        //
        Matrix23 transform;
        transform = translationMatrix2;
        transform *= rotationMatrix;
        transform *= scalingMatrix;
        transform *= translationMatrix1;

        // 3打点
        Vector2 a, b, c;
        transform.multiply(&a, Vector2(0, 0));
        transform.multiply(&b, Vector2(iw, 0));
        transform.multiply(&c, Vector2(0, ih));
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
