#include "GameLib/Framework.h"
#include "GameLib/GameLib.h"
#include "GameLib/Math.h"
#include "Image.h"
#include <sstream>
using namespace GameLib;
using namespace std;
namespace M {
    int round(double a) {
        a += (a > 0.0) ? 0.5 : -0.5f;
        return static_cast<int>(a);
    }

} // namespace M

void rotate(int* rx, int* ry, int x, int y, double xOffset, double yOffset, double sine, double cosine) {
    double xf = static_cast<double>(x);
    double yf = static_cast<double>(y);
    // 合并偏移量和0.5
    double tmpOffsetX = xOffset - 0.5;
    double tmpOffsetY = yOffset - 0.5;
    // 将下标转换为坐标并移动原点
    xf -= tmpOffsetX;
    yf -= tmpOffsetY;
    // 准确应用表达式
    double xf2 = cosine * xf - sine * yf;
    double yf2 = sine * xf + cosine * yf;
    xf = xf2;
    yf = yf2;
    // 将原点还原为下标
    xf += tmpOffsetX;
    yf += tmpOffsetY;
    // 四舍五入为整数
    *rx = M::round(xf);
    *ry = M::round(yf);
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
        double offsetX = static_cast<double>(iw) / 2.0;
        double offsetY = static_cast<double>(ih) / 2.0;
        double rotation = static_cast<double>(gCount);
        double sine = sin(rotation);
        double cosine = cos(rotation);
        for (int y = 0; y < ih; ++y) {
            for (int x = 0; x < iw; ++x) {
                // 计算轮换目的地
                int rx, ry;
                rotate(&rx, &ry, x, y, offsetX, offsetY, sine, cosine);
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
