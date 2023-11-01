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
        // 获取时间（毫秒）
        unsigned time() const;
        // 睡眠（以毫秒为单位）
        void sleep(int milliSeconds) const;
        // 输入
        bool isKeyOn(int c) const;
        /// 获得帧速率
        int frameRate() const;
        /// 帧速率设置（内部可循环等待）
        void setFrameRate(int);
        /// 输入（瞬间）
        bool isKeyTriggered(int c) const;

        // 以下库用户不需要知道
        void start(void* windowHandle);
        void preUpdate();
        void postUpdate();
        static void create();
        static void destroy();
    };

    // cin
    class StandardInput {
      public:
        StandardInput();
        StandardInput& operator>>(char&);

      private:
        bool mPrevEnterOn;
    };
    extern StandardInput cin;

} // namespace GameLib

#include "GameLib/Base/DebugStream.h"

#endif
