#include "GameLib/Framework.h"
#include "GameLib/GameLib.h"
#include "GameLib/Math.h"
#include "Image.h"
#include <sstream>
using namespace GameLib;
using namespace std;

int myround(double a) {
    a += (a > 0.0) ? 0.5 : -0.5f;
    return static_cast<int>(a);
}

void rotate(int* rx, int* ry, int x, int y, double xOffset, double yOffset, double rotation) {
    double xf = static_cast<double>(x);
    double yf = static_cast<double>(y);
    // 将下标转换为坐标
    xf += 0.5f;
    yf += 0.5f;
    // 移动原点
    xf -= xOffset;
    yf -= yOffset;
    // 查找角度和半径
    double r = GameLib::sqrt(xf * xf + yf * yf);
    double angle = GameLib::atan2(yf, xf);
    // 增加旋转角度。
    angle += rotation;
    // 正弦余弦
    double sine = GameLib::sin(angle);
    double cosine = GameLib::cos(angle);
    // 重新计算xf和yf
    xf = r * cosine;
    yf = r * sine;
    // 恢复原点
    xf += xOffset;
    yf += yOffset;
    // 从坐标到下标
    xf -= 0.5;
    yf -= 0.5;
    // 四舍五入为整数
    *rx = myround(xf);
    *ry = myround(yf);
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
        for (int y = 0; y < ih; ++y) {
            for (int x = 0; x < iw; ++x) {
                // 计算轮换目的地
                int rx, ry;
                rotate(&rx, &ry, x, y, offsetX, offsetY, rotation);
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