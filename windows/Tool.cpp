#include <Windows.h>
#include <d3d9.h>

// 全局变量
LPDIRECT3D9 d3d; // Direct3D 对象
LPDIRECT3DDEVICE9 d3dDevice; // Direct3D 设备对象

// 三角形的顶点结构体
struct CUSTOMVERTEX {
    FLOAT x, y, z, rhw;
    DWORD color;
};

// 三角形的顶点数据
CUSTOMVERTEX vertices[] = {
    {320.0f, 50.0f, 1.0f, 1.0f, 0xffff0000}, // x, y, z, rhw, color
    {520.0f, 400.0f, 1.0f, 1.0f, 0xff00ff00},
    {120.0f, 400.0f, 1.0f, 1.0f, 0xff0000ff},
};

// 初始化Direct3D
HRESULT InitD3D(HWND hWnd) {
    // 创建 Direct3D 对象
    if ((d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) return E_FAIL;

    // 设置 Direct3D 的显示模式
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;

    // 创建 Direct3D 设备
    if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice))) { return E_FAIL; }

    return S_OK;
}

// 渲染函数
void Render() {
    // 清空屏幕
    d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    // 开始场景
    if (SUCCEEDED(d3dDevice->BeginScene())) {
        // 设置顶点格式
        d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

        // 绘制三角形
        d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(CUSTOMVERTEX));

        // 结束场景
        d3dDevice->EndScene();
    }

    // 显示场景
    d3dDevice->Present(NULL, NULL, NULL, NULL);
}

// 释放Direct3D资源
void Cleanup() {
    if (d3dDevice != NULL) d3dDevice->Release();

    if (d3d != NULL) d3d->Release();
}

// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 创建窗口
    HWND hWnd;
    WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "DX9Triangle", NULL};
    RegisterClassEx(&wc);
    hWnd = CreateWindow(wc.lpszClassName, "DirectX 9 Triangle", WS_OVERLAPPEDWINDOW, 100, 100, 640, 480, GetDesktopWindow(), NULL, wc.hInstance, NULL);

    // 初始化Direct3D
    if (SUCCEEDED(InitD3D(hWnd))) {
        // 显示窗口
        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        // 主消息循环
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));
        while (msg.message != WM_QUIT) {
            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {
                // 渲染场景
                Render();
            }
        }
    }

    // 释放资源
    Cleanup();

    // 返回退出代码
    return 0;
}