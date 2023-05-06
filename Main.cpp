#include <Windows.h>
#include <DirectXMath.h>
#include <D3Dx11tex.h>
#include "DXdemo.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "D3DX11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// 程序入口点
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // 创建 Win32 窗口
    HWND hWnd;
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(wcex));
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = DefWindowProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = "D3D11Sample";

    RegisterClassEx(&wcex);

    hWnd = CreateWindow("D3D11Sample", "Direct3D 11 Sample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 7680,3840, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, nShowCmd);

    DXdemo  demo(hInstance, 7680, 3840);
    
    if (!demo.Init(hWnd, 7680, 3840)) {
        return 0;
    }

    // 进入消息循环
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            demo.Render();
        }
    }

    // 释放资源
    demo.Cleanup();
    return 0;
}


