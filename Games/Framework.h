#ifndef INCLUDED_GAMELIB_FRAMEWORK_H
#define INCLUDED_GAMELIB_FRAMEWORK_H
#include "../libs/libpng/png.h"
#include "../lua/src/lua.hpp"
//
#include "../extensions/luafilesystem/src/lfs.h"
//
#include <ft2build.h>
#include FT_FREETYPE_H

namespace GameLib {

    class Framework {
      public:
        Framework();
        /// 用户定义函数。
        void update();
        /// 获取实例
        static Framework getInstance();
        static Framework instance();
        unsigned* Framework::getVideoMemory();
        /// 获取画面宽度
        int width() const;
        int getWidth() const;

        /// 获取画面高度
        int height() const;
        int getHeight() const;
        // 获取VRAM
        unsigned* videoMemory();
        // 向Framework发出结束命令
        void requestEnd();
        // 检查Framework是否发出结束命令
        bool isEndRequested() const;
        //
        lua_State* getLuaState();
        // 获取时间（毫秒）
        unsigned time() const;
        // 睡眠（以毫秒为单位）
        void sleep(int milliSeconds) const;
        // 输入
        bool isKeyOn(int c) const;
        /// 输入（瞬间）
        bool isKeyTriggered(int c) const;
        /// 帧速率设置（内部可循环等待）
        void setFrameRate(int);
        /// 获得帧速率
        int frameRate() const;

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

#include "include/GameLib/Base/DebugStream.h"

#endif
