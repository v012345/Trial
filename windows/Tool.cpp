#include <windows.h>

HWND hButton = NULL;
HWND hEdit = NULL; // 输入框句柄
HWND hLog = NULL; // 日志文本框句柄

void Log(const wchar_t* message) {
    // 获取日志文本框的当前文本长度
    int len = GetWindowTextLength(hLog);

    // 设置日志文本框的选择范围为文本的末尾
    SendMessage(hLog, EM_SETSEL, len, len);

    // 在日志文本框中追加消息
    SendMessage(hLog, EM_REPLACESEL, 0, (LPARAM)message);

    // 滚动日志文本框以显示最新消息
    SendMessage(hLog, EM_SCROLL, SB_LINEDOWN, 0);
}

void SimulateTimeConsumingOperation() {
    Log(L"Starting operation...\r\n");

    // 模拟一个耗时操作，这里使用 Sleep 函数暂停程序执行
    // Sleep(5000); // 5秒

    Log(L"Operation completed.\r\n");
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE: PostQuitMessage(0); break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            TextOut(hdc, 10, 10, L"Hello, World!", 12);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                EnableWindow(hButton, FALSE);
                SimulateTimeConsumingOperation();
                EnableWindow(hButton, TRUE);
            }
            break;
        default: return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t className[] = L"HelloWorldClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (RegisterClass(&wc)) {
        HWND hwnd = CreateWindow(className, L"Tool", WS_OVERLAPPEDWINDOW, 100, 100, 600, 400, NULL, NULL, hInstance, NULL);

        if (hwnd) {
            ShowWindow(hwnd, nCmdShow);
            UpdateWindow(hwnd);

            // 创建一个按钮
            hButton = CreateWindow(L"BUTTON", L"replace", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 40, 100, 30, hwnd, (HMENU)1, hInstance, NULL);

            // 创建一个输入框
            hEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 10, 80, 200, 30, hwnd, (HMENU)2, hInstance, NULL);

            // 创建一个多行文本框用于日志
            hLog = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | ES_MULTILINE, 10, 120, 360, 220, hwnd, (HMENU)3, hInstance, NULL);

            MSG msg;
            while (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return 0;
}