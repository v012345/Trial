#include "GameLib/Framework.h"
#include "GameLib/GameLib.h"
#include "GameLib/Math.h"
#include "Image.h"
#include "Vector2.h"
#include <sstream>
using namespace GameLib;
using namespace std;
namespace M {
    int round(double a) {
        a += (a > 0.0) ? 0.5 : -0.5f;
        return static_cast<int>(a);
    }
} // namespace M

void rotate(int* rx, int* ry, int x, int y, const Vector2& offset, double sine, double cosine) {
    Vector2 p(x, y);
    // 合并偏移量和0.5
    Vector2 tmpOffset(-0.5, -0.5);
    tmpOffset += offset; // tmpOffset = offset-(0.5,0.5)
    // 将下标转换为坐标并移动原点
    p -= tmpOffset;
    // 准确应用表达式
    Vector2 r;
    r.x = cosine * p.x - sine * p.y;
    r.y = sine * p.x + cosine * p.y;
    // 将原点还原为下标
    r += tmpOffset;
    // 四舍五入为整数
    *rx = M::round(r.x);
    *ry = M::round(r.y);
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
        Vector2 offset;
        offset.x = static_cast<double>(iw) / 2.0;
        offset.y = static_cast<double>(ih) / 2.0;
        double rotation = static_cast<double>(gCount);
        double sine = sin(rotation);
        double cosine = cos(rotation);
        for (int y = 0; y < ih; ++y) {
            for (int x = 0; x < iw; ++x) {
                // 计算轮换目的地
                int rx, ry;
                rotate(&rx, &ry, x, y, offset, sine, cosine);
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
