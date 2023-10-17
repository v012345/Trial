#include "GameLib/GameLib.h"
//
#include "GameLib/FileIO/Manager.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Scene/Font.h"
#include "GameLib/Scene/StringRenderer.h"
#include "GameLib/Sound/Manager.h"
#include "GameLib/Threading/Functions.h"
#include "GameLib/Threading/Manager.h"
#include "GameLib/WindowCreator/WindowCreator.h"
// #include "GameLib/Base/Array.h"

// 示例类库
#include "GameLib/Input/Keyboard.h"

#include "FontTextureGenerated.h"
#include "Framework.h"

#include <string>
using namespace std;

namespace GameLib {

    namespace { // 匿名

        class Impl {
          public:
            Impl()
                : mWidth(320), mHeight(240), mFullScreen(false), mFullScreenForbidden(true), mVSync(false), mAntiAlias(false), mTitle("A GameLib Framework Application"), mArchiveNames(0),
                  mArchiveNumber(0), mLoadMode(FileIO::Manager::MODE_DIRECT_FIRST), mPreviousFrameInterval(0), mFrameRate(0), mEndRequested(false), mStarted(false) {
                // cout初始化
                cout.begin();
                // 帧历史记录重置
                unsigned t = time();
                for (int i = 0; i < TIME_HISTORY_SIZE; ++i) { mTimeHistory[i] = t; }

                mVideoMemoryWithPadding.setSize(mWidth * (mHeight + 2));
                // 0初始化
                for (int i = 0; i < mWidth * (mHeight + 2); ++i) { mVideoMemoryWithPadding[i] = 0; }
                for (int i = 0; i < mWidth; ++i) {
                    mVideoMemoryWithPadding[i] = MAGIC_NUMBER;
                    mVideoMemoryWithPadding[mWidth * (mHeight + 1) + i] = MAGIC_NUMBER;
                }
            }
            ~Impl() {
                if (mArchiveNames) { SAFE_DELETE_ARRAY(mArchiveNames); }
                mDebugStringRenderer.release();
                mDebugFont.release();
                Graphics::Manager::destroy();
                Input::Manager::destroy();
                Sound::Manager::destroy();
                FileIO::Manager::destroy();
                Threading::Manager::destroy();
                // cout结束
                cout.end();
            }
            void start(void* windowHandle) {
                // 线程系统初始化
                Threading::Manager::create(0);
                // 初始化文件加载器
                if (mArchiveNumber > 0) {
                    Array<const char*> names(mArchiveNumber);
                    for (int i = 0; i < mArchiveNumber; ++i) { names[i] = mArchiveNames[i].c_str(); }
                    FileIO::Manager::create(&names[0], mArchiveNumber, mLoadMode);
                    // 丢弃，因为不需要归档名称数组
                    SAFE_DELETE_ARRAY(mArchiveNames);
                    mArchiveNumber = 0;
                } else {
                    FileIO::Manager::create();
                }
                // 声音初始化
                Sound::Manager::create(windowHandle);
                // 输入初始化
                Input::Manager::create(windowHandle);
                // 初始化绘制
                Graphics::Manager::create(windowHandle, mWidth * 2, mHeight * 2, mFullScreen, mVSync, mAntiAlias);
                // 绘制文字
                // ASCII表（ASCII（126-32 + 1 = 95个字符+ 63个字符，158个字符
                Array<Scene::Font::BitmapChar> chars(158);
                // ASCII部分填充
                int pos = 0;
                for (int i = 0x20; i <= 0x7E; ++i) {
                    Scene::Font::BitmapChar& c = chars[pos];
                    c.mCode = static_cast<unsigned short>(i);
                    c.mX = static_cast<unsigned short>((i % 16) * 8);
                    c.mY = static_cast<unsigned short>((i / 16) * 16);
                    c.mTexturePage = 0;
                    c.mWidth = 8;
                    c.mHeight = 16;
                    ++pos;
                }
                // 半角假名部分
                for (int i = 0xA1; i <= 0xDF; ++i) {
                    Scene::Font::BitmapChar& c = chars[pos];
                    c.mCode = static_cast<unsigned short>(i);
                    c.mX = static_cast<unsigned short>((i % 16) * 8);
                    c.mY = static_cast<unsigned short>((i / 16) * 16);
                    c.mTexturePage = 0;
                    c.mWidth = 8;
                    c.mHeight = 16;
                    ++pos;
                }

                ASSERT(pos == 158);
                int textureSize = sizeof(gFontTga);
                const char* fontDataArray[1];
                fontDataArray[0] = gFontTga;
                mDebugFont = Scene::Font::create(fontDataArray, &textureSize, 1, &(chars[0]), 158);
                mDebugStringRenderer = Scene::StringRenderer::create(2048, 128); // 这样够了吗
                mDebugStringRenderer.setFont(mDebugFont);
                // 2D图层
                m2dTexture = Graphics::Texture::create(mWidth, mHeight, false);
                Graphics::Manager().setTextureFilter(Graphics::TEXTURE_FILTER_POINT);

                mStarted = true;
            }
            unsigned time() const { return WindowCreator().time(); }
            void preUpdate() {
                // 帧时间更新
                unsigned currentTime = time();
                mPreviousFrameInterval = currentTime - mTimeHistory[TIME_HISTORY_SIZE - 1];
                unsigned frameIntervalSum = currentTime - mTimeHistory[0];
                mFrameRate = TIME_HISTORY_SIZE * 1000 / frameIntervalSum;
                // 更新历史
                for (int i = 0; i < TIME_HISTORY_SIZE - 1; ++i) { mTimeHistory[i] = mTimeHistory[i + 1]; }
                mTimeHistory[TIME_HISTORY_SIZE - 1] = currentTime;

                Graphics::Manager().beginDraw();
                float pointerScale;
                Vector2 pointerOffset;
                Graphics::Manager().getPointerModifier(&pointerScale, &pointerOffset);
                Input::Manager().update(pointerScale, pointerOffset);
            }
            void postUpdate() {
                //----2D处理
                // 检查销毁
                for (int i = 0; i < mWidth; ++i) {
                    ASSERT(mVideoMemoryWithPadding[i] == MAGIC_NUMBER && "VRAM RANGE ERROR!");
                    ASSERT(mVideoMemoryWithPadding[mWidth * (mHeight + 1) + i] == MAGIC_NUMBER && "VRAM RANGE ERROR!");
                }
                // 复制VRAM
                unsigned* dst;
                int pitch;
                m2dTexture.lock(&dst, &pitch);
                const unsigned* src = &mVideoMemoryWithPadding[mWidth];
                for (int y = 0; y < mHeight; ++y) {
                    for (int x = 0; x < mWidth; ++x) {
                        dst[x] = 0xff000000 | src[x]; // 强制alpha ff
                    }
                    src += mWidth;
                    dst += pitch / 4;
                }
                src = 0; // 使用终止
                m2dTexture.unlock(&dst);
                Graphics::Manager().blendToScreen(m2dTexture);

                // 合成文字
                mDebugStringRenderer.draw();
                Graphics::Manager().endDraw();
            }
            // 示例类库
            Array<unsigned> mVideoMemoryWithPadding;
            Graphics::Texture m2dTexture;
            static const unsigned MAGIC_NUMBER = 0x12345678;

            int mWidth;
            int mHeight;
            bool mFullScreen;
            bool mFullScreenForbidden;
            bool mVSync;
            bool mAntiAlias;
            string mTitle;
            string* mArchiveNames;
            int mArchiveNumber;
            FileIO::Manager::AccessMode mLoadMode;
            static const int TIME_HISTORY_SIZE = 60;
            unsigned mTimeHistory[TIME_HISTORY_SIZE];
            int mPreviousFrameInterval;
            int mFrameRate;
            bool mEndRequested;
            bool mStarted;
            Scene::StringRenderer mDebugStringRenderer;
            Scene::Font mDebugFont;
        };

        Impl* gImpl = 0;
        lua_State* L = nullptr;
    } // namespace
    static int lua_getHeight(lua_State* L) {
        Impl** ppImpl = (Impl**)lua_touserdata(L, 1);
        lua_pushinteger(L, (*ppImpl)->mHeight);
        return 1;
    }
    static int lua_getWidth(lua_State* L) {
        Impl** ppImpl = (Impl**)lua_touserdata(L, 1);
        lua_pushinteger(L, (*ppImpl)->mWidth);
        return 1;
    }
    static int lua_setVarm(lua_State* L) {
        Impl** ppImpl = (Impl**)lua_touserdata(L, 1);
        unsigned* vram = &gImpl->mVideoMemoryWithPadding[gImpl->mWidth];
        unsigned w = lua_tointeger(L, 2);
        unsigned h = lua_tointeger(L, 3);
        unsigned c = lua_tointeger(L, 4);
        vram[w * gImpl->mWidth + h] = c;
        return 0;
    }
    static int luaopen_Impl(lua_State* L, Impl* gImpl) {
        luaL_Reg pImpl_metatable[] = {
            {"height", lua_getHeight},
            {"width", lua_getWidth},
            {"vram", lua_setVarm},
            {NULL, NULL},
        };
        Impl** ppImpl = (Impl**)lua_newuserdata(L, sizeof(Impl**));
        *ppImpl = gImpl;
        luaL_newmetatable(L, "pImpl_metatable");
        lua_newtable(L);
        luaL_setfuncs(L, pImpl_metatable, 0);
        lua_setfield(L, -2, "__index");
        lua_setmetatable(L, -2);
        lua_setglobal(L, "Impl");
        return 1;
    }
    lua_State* Framework::getLuaState() { return L; }
    Framework::Framework() {
        // 不允许来自其他线程的调用
        ASSERT(WindowCreator::isMainThread() && "you must call from MAIN thread");
    }

    void Framework::create() {
        ASSERT(!gImpl);
        gImpl = NEW Impl();
        L = luaL_newstate();
        luaL_openlibs(L);
        luaopen_lfs(L);
#ifdef CMAKE_SOURCE_DIR
        lua_pushstring(L, CMAKE_SOURCE_DIR);
        lua_setglobal(L, "CMAKE_SOURCE_DIR");
#endif

#ifdef LUA_PACKAGE_PATH
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "path");
        const char* path = lua_tostring(L, -1);
        char* new_path = (char*)malloc(sizeof(char) * (strlen(path) + strlen(";" LUA_PACKAGE_PATH) + 1));
        memcpy(new_path, path, strlen(path));
        memcpy(new_path + strlen(path), ";" LUA_PACKAGE_PATH, strlen(";" LUA_PACKAGE_PATH) + 1);
        lua_pop(L, 1);
        lua_pushstring(L, "path");
        lua_pushstring(L, new_path);
        lua_settable(L, -3);
        lua_pop(L, 1);
        free(new_path);
#endif
        luaopen_Impl(L, gImpl);
#ifdef LUA_MAIN_SCRIPT
        luaL_dofile(L, LUA_MAIN_SCRIPT);
#endif
    }

    void Framework::destroy() { SAFE_DELETE(gImpl); }

    Framework Framework::instance() { return Framework(); }

    void Framework::start(void* windowHandle) { gImpl->start(windowHandle); }

    void Framework::preUpdate() { gImpl->preUpdate(); }

    void Framework::postUpdate() { gImpl->postUpdate(); }

    int Framework::width() const { return gImpl->mWidth; }

    int Framework::height() const { return gImpl->mHeight; }
    /*
    const char* Framework::getTitle() const {
            return gImpl->mTitle.c_str();
    }

    bool Framework::isFullScreen() const {
            return gImpl->mFullScreen;
    }

    bool Framework::isFullScreenForbidden() const {
            return gImpl->mFullScreenForbidden;
    }
    */
    void Framework::requestEnd() { gImpl->mEndRequested = true; }

    bool Framework::isEndRequested() const { return gImpl->mEndRequested; }
    /*
    unsigned Framework::getTime() const {
            return gImpl->getTime();
    }

    void Framework::sleep( int ms ) const {
            WindowCreator::getInstance().sleep( ms );
    }

    int Framework::getPreviousFrameInterval() const {
            return gImpl->mPreviousFrameInterval;
    }

    int Framework::getFrameRate() const {
            return gImpl->mFrameRate;
    }

    void Framework::drawDebugString( int c, int r, const char* s, unsigned col ){
            gImpl->mDebugStringRenderer.add( c * 8, r * 16, s, col );
    }

    void Framework::enableFullScreen( bool f ){
            if ( gImpl->mFullScreenForbidden ){ //被禁止
                    return;
            }
            WindowCreator wc = WindowCreator::getInstance();
            Graphics::Manager gm = Graphics::Manager::getInstance();
            if ( gImpl->mStarted ){
                    //中途更改
                    if ( gImpl->mFullScreen != f ){
                            //上游
                            wc.enableFullScreen( f );
                            //下游
                            gm.enableFullScreen( f );
                    }
            }
            gImpl->mFullScreen = f;
    }

    //Configuration
    void Framework::Configuration::setWidth( int width ){
            gImpl->mWidth = width;
    }

    void Framework::Configuration::setHeight( int height ){
            gImpl->mHeight = height;
    }

    void Framework::Configuration::setTitle( const char* title ){
            gImpl->mTitle = title;
    }

    void Framework::Configuration::setArchiveNumber( int n ){
            ASSERT( !gImpl->mArchiveNames && "you can't call twice." );
            gImpl->mArchiveNumber = n;
            gImpl->mArchiveNames = NEW string[ n ];
    }

    void Framework::Configuration::setArchiveName( int index, const char* name ){
            ASSERT( gImpl->mArchiveNames && "you must call setArchiveNumber() before this call." );
            ASSERT( index >= 0 && index < gImpl->mArchiveNumber );
            gImpl->mArchiveNames[ index ] = name;
    }

    void Framework::Configuration::setLoadMode( LoadMode lm ){
            using namespace FileIO;
            Manager::AccessMode am = Manager::MODE_DIRECT_FIRST;
            switch ( lm ){
                    case LOAD_ARCHIVE_FIRST: am = Manager::MODE_ARCHIVE_FIRST; break;
                    case LOAD_DIRECT_FIRST: am = Manager::MODE_DIRECT_FIRST; break;
                    case LOAD_ARCHIVE_ONLY: am = Manager::MODE_ARCHIVE_ONLY; break;
                    default: ASSERT( false ); break;
            }
            gImpl->mLoadMode = am;
    }

    void Framework::Configuration::enableFullScreen( bool f ){
            gImpl->mFullScreen = f;
    }

    void Framework::Configuration::enableVSync( bool f ){
            gImpl->mVSync = f;
    }

    void Framework::Configuration::enableAntiAlias( bool f ){
            gImpl->mAntiAlias = f;
    }

    void Framework::Configuration::forbidFullScreen( bool f ){
            gImpl->mFullScreenForbidden = f;
    }
    */

    // 示例类库使用的函数
    unsigned* Framework::videoMemory() { return &gImpl->mVideoMemoryWithPadding[gImpl->mWidth]; }

    // WindowProcedure的用户封装函数
    void WindowCreator::configure(Configuration* config) {
        Framework::create();
        // 用户设定
        Framework f;
        //	Framework::Configuration fwConfig;
        //	f.configure( &fwConfig );
        config->setWidth(f.width() * 2);
        config->setHeight(f.height() * 2);
        config->setTitle("2D Graphics1 samples");
        config->enableFullScreen(false);
        config->forbidFullScreen(true);
    }

    void WindowCreator::update() {
        WindowCreator wc;
        Graphics::Manager gm;
        Framework f;
        // 根据WindowCreator的状态执行各种操作
        bool sleepFlag = false;
        // 最小化后则休眠
        if (wc.isMinimized()) { sleepFlag = true; }
        // 基本上会进入睡眠状态。
        /*
                if ( !gm.canRender() ){
                        gm.restore();
                        sleepFlag = true;
                }
                //来自Window的模式切换信号
                bool wcFullScreen = wc.isFullScreen();
                if ( f.isFullScreen() != wcFullScreen ){
                        f.enableFullScreen( wcFullScreen );
                }
        */
        if (!sleepFlag) { f.preUpdate(); }
        // 检查结束命令是否来自上游（例如x按钮）
        if (isEndRequested()) { // 收到后则将终止命令发送到Framework
            f.requestEnd();
        }
        if (sleepFlag) {
            Threading::sleep(50); // 睡50ms
        } else {
            f.update();
            f.postUpdate();
        }
        // 结束判断
        if (f.isEndRequested()) {
            Framework::destroy();
            end(); // 让上游知道
        }
    }

    void WindowCreator::start(void* windowHandle) { Framework().start(windowHandle); }

    // StandardInput
    StandardInput cin;

    StandardInput::StandardInput() : mPrevEnterOn(false) {}

// 这里直接调用Windows功能
#include <windows.h>
    StandardInput& StandardInput::operator>>(char& a) {
        a = '\0'; // 初始值
        bool entered = false;
        while (true) { // 无限循环
            // 空格
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
                a = ' ';
                entered = true;
            }
            // 0-9
            for (char i = '0'; i <= '9'; ++i) {
                if (GetAsyncKeyState(i) & 0x8000) {
                    a = i;
                    entered = true;
                }
            }
            // a-z
            for (char i = 'A'; i <= 'Z'; ++i) {
                if (GetAsyncKeyState(i) & 0x8000) {
                    if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                        a = i;
                    } else {
                        a = 'a' + (i - 'A');
                    }
                    entered = true;
                }
            }
            bool enterOn = ((GetAsyncKeyState(VK_RETURN) & 0x8000) != 0);
            if (entered && enterOn && !mPrevEnterOn) {
                mPrevEnterOn = enterOn;
                break;
            }
            // 如果收到终止请求则退出
            bool alt = ((GetAsyncKeyState(VK_MENU) & 0x8000) != 0);
            bool f4 = ((GetAsyncKeyState(VK_F4) & 0x8000) != 0);
            if (alt && f4) {
                Framework().requestEnd();
                break;
            }
            mPrevEnterOn = enterOn;
        }
        return *this;
    }

} // namespace GameLib
