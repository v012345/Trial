#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x400 //WM_MOUSEWHEEL
#endif

#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>
#include "WindowCreator/resource.h"
#include <string>
#undef min
#undef max

#include "GameLib/GameLib.h"
#include "GameLib/WindowCreator/WindowCreator.h"
#include "GameLib/Threading/Functions.h"

using namespace std;
using namespace GameLib;

//WindowCreator成员函数
namespace GameLib{

namespace{ //匿名

class Impl{
public:
	Impl() :
	mWidth( 640 ), 
	mHeight( 480 ), 
	mFullScreen( false ),
	mMinimized( false ),
	mActive( false ),
	mTitle( "A GameLib Application" ),
	mCommandLine( "" ),
	mEndRequested( false ),
	mEnded( false ),
	mStarted( false ),
	mFullScreenForbidden( false ),
	mDragAndDropEnabled( false ),
	mMouseWheel( 0 ),
	mTimeBias( 0 ){
		//计算getTime的偏差值（）
		unsigned time = timeGetTime();
		mTimeBias = 0xffffffff - time;
		mTimeBias -= 60000; //60秒。

		//获取线程ID
		mThreadId = GetCurrentThreadId();
	}
	~Impl(){
	}
	void enableFullScreen( bool f ){
		if ( mFullScreen != f ){
			if ( mStarted ){
				LONG style;
				if ( f ){
					style = WS_POPUP | WS_VISIBLE;
					//最大化时记住的当前位置
					GetWindowRect( mWindowHandle, &mWindowRect );
					SetWindowLong( mWindowHandle, GWL_STYLE, style );
					//位置是0,0
					SetWindowPos( mWindowHandle, NULL, 
						0, 0,
						mWidth, mHeight,
						SWP_SHOWWINDOW );
				}else{
					style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;
					SetWindowLong( mWindowHandle, GWL_STYLE, style );
					//撤消时恢复位置
					SetWindowPos( mWindowHandle, NULL, 
						mWindowRect.left, mWindowRect.top,
						mWindowRect.right - mWindowRect.left,
						mWindowRect.bottom - mWindowRect.top,
						SWP_SHOWWINDOW );
				}
			}
			mFullScreen = f;
		}
	}
	void createDefaultWindowPos(){
		mWindowRect.left = 0;
		mWindowRect.top = 0;
		mWindowRect.right = mWidth;
		mWindowRect.bottom = mHeight;
		LONG style;
		style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;
		int oldLeft = mWindowRect.left;
		int oldTop = mWindowRect.top;
		AdjustWindowRect( &mWindowRect, style, FALSE );
		int diffX = mWindowRect.left - oldLeft;
		int diffY = mWindowRect.top - oldTop;
		mWindowRect.left -= diffX;
		mWindowRect.right -= diffX;
		mWindowRect.top -= diffY;
		mWindowRect.bottom -= diffY;
	}
	int mWidth;
	int mHeight;
	bool mFullScreen;
	bool mMinimized;
	bool mActive;
	string mTitle;
	string mCommandLine;
	bool mEndRequested;
	bool mEnded;
	bool mStarted;
	bool mFullScreenForbidden;
	bool mDragAndDropEnabled;
	int mMouseWheel;
	unsigned mTimeBias; //可用于获取timeGetTime
	unsigned mThreadId;
	Array< string > mDroppedItems;
	HWND mWindowHandle;
	RECT mWindowRect;
};

Impl* gImpl = 0; //唯一的实例

} //namespace{}

WindowCreator::WindowCreator(){
	//不允许来自其他线程的调用
	ASSERT( isMainThread() && "you must call from MAIN thread" );
}

const char* WindowCreator::title() const {
	return gImpl->mTitle.c_str();
}

const char* WindowCreator::commandLineString() const {
	return gImpl->mCommandLine.c_str();
}

int WindowCreator::titleLength() const {
	return static_cast< int >( gImpl->mTitle.size() );
}

int WindowCreator::height() const {
	return gImpl->mHeight;
}

int WindowCreator::width() const {
	return gImpl->mWidth;
}

bool WindowCreator::isFullScreen() const {
	return gImpl->mFullScreen;
}

WindowCreator WindowCreator::instance(){
	return WindowCreator(); //不需要这个功能
}

bool WindowCreator::isEndRequested() const {
	return gImpl->mEndRequested;
}

void WindowCreator::requestEnd(){
	gImpl->mEndRequested = true;
}

void WindowCreator::end(){
	gImpl->mEnded = true;
}

unsigned WindowCreator::time() const {
	return timeGetTime() + gImpl->mTimeBias;
}

bool WindowCreator::isMinimized() const {
	return gImpl->mMinimized;
}

bool WindowCreator::isActive() const {
	return gImpl->mActive;
}

bool WindowCreator::isMainThread(){
	return ( GetCurrentThreadId() == gImpl->mThreadId );
}

void WindowCreator::enableFullScreen( bool f ){
	gImpl->enableFullScreen( f );
}

bool WindowCreator::isDragAndDropEnabled() const {
	return gImpl->mDragAndDropEnabled;
}

int WindowCreator::droppedItemNumber() const {
	return gImpl->mDroppedItems.size();
}

const char* WindowCreator::droppedItem( int i ) const {
	return gImpl->mDroppedItems[ i ].c_str();
}

void WindowCreator::clearDroppedItem(){
	gImpl->mDroppedItems.clear();
}

int WindowCreator::getAndResetMouseWheel(){
	int t = gImpl->mMouseWheel;
	gImpl->mMouseWheel = 0;
	return t;
}

//Configuration
void WindowCreator::Configuration::setWidth( int w ){
	gImpl->mWidth = w;
}

void WindowCreator::Configuration::setHeight( int h ){
	gImpl->mHeight = h;
}

void WindowCreator::Configuration::setTitle( const char* title ){
	gImpl->mTitle = title;
}

void WindowCreator::Configuration::enableFullScreen( bool f ){
	gImpl->enableFullScreen( f );
}

void WindowCreator::Configuration::forbidFullScreen( bool f ){
	gImpl->mFullScreenForbidden = f;
}

void WindowCreator::Configuration::enableDragAndDrop( bool f ){
	gImpl->mDragAndDropEnabled = f;
}

} //namespace GameLib

#define MAX_LOADSTRING 100

// 全局变量：
HINSTANCE hInst;								// 当前接口
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏中的文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名称

// 此代码模块中包含的函数声明：
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CoInitializeEx( NULL, COINIT_MULTITHREADED );
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	//TODO：请在这里插入代码。
	MSG msg;
	HACCEL hAccelTable;

	//生成窗口状态维护类
	gImpl = NEW Impl();
	gImpl->mCommandLine = lpCmdLine;
	WindowCreator wc;
	//呼叫用户设定功能
	WindowCreator::Configuration config;
	wc.configure( &config );

	memcpy(
		szTitle, 
		wc.title(), 
		wc.titleLength() ); 
	memcpy( szWindowClass, "GameLibWindowCreator", sizeof("GameLibWindowCreator"));

	MyRegisterClass( hInstance );

	// 执行应用程序初始化：
	if ( !InitInstance ( hInstance, nCmdShow ) ){
		return FALSE;
	}

	hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_WINDOWCREATOR ) );

	timeBeginPeriod( 1 );
	HWND windowHandle = gImpl->mWindowHandle;
	///启动前出现异常无法继续，弹出消息框然后结束
	try{
		wc.start( windowHandle );
	}
	catch ( ... ){ //启动时不能忽略异常。
		MessageBoxA( 
			windowHandle, 
			"致命的な問題があり、起動できません。申し訳ありませんが終了いたします", 
			"致命的エラー", 
			MB_OK | MB_ICONERROR );
		wc.requestEnd(); //结束
		wc.update(); //结束处理
		SAFE_DELETE( gImpl );
		timeEndPeriod( 1 );
		return 0;
	}

	//
	if ( gImpl->mFullScreen ){
		gImpl->createDefaultWindowPos();
	}
	//启用拖放
	if ( gImpl->mDragAndDropEnabled ){
		DragAcceptFiles( windowHandle, TRUE );
	}
	gImpl->mStarted = true; //循环启动信号
	while ( true ){
		//有消息时处理
		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ){
			if ( GetMessage( &msg, NULL, 0, 0 ) ){
				if ( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) ){
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}else{
				break; //收到WM_QUIT，跳出循环。
			}
		}
		//如果结束了，销毁window
		if ( gImpl->mEnded ){ //如果没有结束则更新循环
			if ( windowHandle ){ //也许两次，以防万一
				DestroyWindow( windowHandle );
				windowHandle = 0;
			}
		}else{	//
			try{
				wc.update();
			}
			catch ( Exception e ){
				if ( e == EXCEPTION_EXIT ){ //仅在终止请求的情况下终止
					MessageBoxA( 
						windowHandle, 
						// "致命的な問題があり、継続できません。申し訳ありませんが終了いたします", 
						// "致命的エラー", 
						"There is a fatal problem and we cannot continue. We are sorry but this will end.", 
						"fatal error", 
						MB_OK | MB_ICONERROR );
				}
			}
		}
	}
	SAFE_DELETE( gImpl );
	timeEndPeriod( 1 );

	CoUninitialize();
	return (int) msg.wParam;
}



//
//  函数：MyRegisterClass（）
//
//  目的：注册窗口类。
//
//  注释：
//
//    该函数和使用方法追加了“RegisterClassEx”函数
//只有与Windows 95之前的Win 32系统兼容时，才需要
//    应用程序已关联
//    为了获得格式正确的小图标，
//    请调用此函数。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWCREATOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WINDOWCREATOR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数：InitInstance（HINSTANCE，int）
//
//   目的：保存实例以创建主窗口。
//
//   注释：
//
//        使用此函数，在全局变量中保存实例
//        创建并显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 在全局变量中存储实例
//----
	DWORD style = 0;
	if ( gImpl->mFullScreen ){
		style = WS_POPUP;
	}else{
		style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	}
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = gImpl->mWidth;
	rect.bottom = gImpl->mHeight;
	AdjustWindowRect(&rect, style, FALSE);
	rect.right -= rect.left;
	rect.bottom -= rect.top; //计算差
	rect.left = 0;
	rect.top = 0;
//----

	hWnd = CreateWindow(szWindowClass, szTitle, 
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right, rect.bottom, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   gImpl->mWindowHandle = hWnd;

   return TRUE;
}

//
//  函数：WndProc（HWND、UINT、WPARAM、LPARAM）
//
//  目的：处理主窗口的信息。
//
//WM COMMAND-应用程序菜单的处理
//WM PAINT-主窗口的绘制
//WM DESTROY-显示中止消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		break;
	case WM_CLOSE: //终止请求
		gImpl->mEndRequested = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SYSKEYDOWN: //识别alt +回车
		if ( !gImpl->mFullScreenForbidden ){
			if ( wParam == VK_RETURN ){
				if ( gImpl->mFullScreen ){
					WindowCreator().enableFullScreen( false );
				}else{
					WindowCreator().enableFullScreen( true );
				}
			}
		}
		DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_ACTIVATE:
		if ( wParam & 0xffff0000 ){ //如果最小化则为false
			gImpl->mActive = false;
		}else{
			gImpl->mActive = ( ( wParam & 0xffff ) != 0 );
		}
		break;
	case WM_SYSCOMMAND:
		if ( wParam == SC_MAXIMIZE ){
			if ( !gImpl->mFullScreenForbidden ){
				WindowCreator().enableFullScreen( true ); //最大化时不要调用DefWindowProc
			}
		}else if ( wParam == SC_MINIMIZE ){
			gImpl->mMinimized = true;
			gImpl->mActive = false;
			DefWindowProc(hWnd, message, wParam, lParam);
		}else if ( wParam == SC_RESTORE ){
			gImpl->mMinimized = false;
			gImpl->mActive = true;
			DefWindowProc(hWnd, message, wParam, lParam);
		}else{
			DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MOUSEWHEEL:
		gImpl->mMouseWheel = ( static_cast< short >( HIWORD( wParam ) ) > 0 ) ? 1 : -1;
		break;
	case WM_DROPFILES:
		if ( gImpl->mDragAndDropEnabled ){
			HDROP hDrop = ( HDROP )wParam;
			gImpl->mDroppedItems.clear();
			unsigned n = DragQueryFileA( hDrop, 0xffffffff, NULL, 0 );
			gImpl->mDroppedItems.setSize( n );
			char item[ MAX_PATH ];
			for ( unsigned i = 0; i < n; ++i ){
				DragQueryFileA( hDrop, i, item, MAX_PATH );
				gImpl->mDroppedItems[ i ] = item;
			}
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
