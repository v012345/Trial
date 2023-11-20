#ifndef INCLUDED_GAMELIB_FRAMEWORK_H
#define INCLUDED_GAMELIB_FRAMEWORK_H

namespace GameLib {

    class Framework {
      public:
        Framework();
        /// 用户定义函数。
        void update();
        /// 获取实例
        static Framework instance();
        /// 获取画面宽度
        int width() const;
        /// 获取画面高度
        int height() const;
        // 获取VRAM
        unsigned* videoMemory();
        // 向Framework发出结束命令
        void requestEnd();
        // 检查Framework是否发出结束命令
        bool isEndRequested() const;
        /// 以毫秒为单位休眠。
        void sleep(int milliSeconds) const;
        /// 获取时间（以毫秒计）
        unsigned time() const;
        /// 获得帧速率
        int frameRate() const;
        /// 帧速率设置（内部可循环等待）
        void setFrameRate(int);
        /// 获取上一帧的时间（以毫秒为单位）
        int previousFrameInterval() const;
        /// 随机数
        int getRandom(int max = 0);
        // 绘制字符
        void drawDebugString(
            int column, // 列（x）
            int row, // 行（y）
            const char* string, unsigned color = 0xffffffff);

        // 以下库用户不需要知道
        void start(void* windowHandle);
        void preUpdate();
        void postUpdate();
        static void create();
        static void destroy();
    };

} // namespace GameLib

#include "GameLib/GameLib.h"

#endif
