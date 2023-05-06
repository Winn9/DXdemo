#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include <DirectXMath.h>

class DXdemo
{
public:
	DXdemo(HINSTANCE hInstance, int initWidth, int initHeight);
	~DXdemo();
	bool Init(HWND m_hMainWnd, int g_WindowWidth, int g_WindowHeight);
	void Render();
	void Cleanup();
protected:
	bool InitD3D(HWND m_hMainWnd, int g_WindowWidth, int g_WindowHeight);
	bool InitEffect();
	bool InitResource();

public:
	HINSTANCE m_hAppInst;        // 应用实例句柄
	HWND      m_hMainWnd;        // 主窗口句柄

	// 应用程序窗口的宽度和高度
	int g_WindowWidth = 7680;
	int g_WindowHeight = 3840;
	// Direct3D 11 的设备和设备上下文
	ID3D11Device* g_pd3dDevice;
	ID3D11DeviceContext* g_pd3dDeviceContext;
	// 用于渲染的顶点和索引缓冲区
	ID3D11Buffer* g_pVertexBuffer;
	ID3D11Buffer* g_pIndexBuffer;
	// 顶点着色器和像素着色器
	ID3D11VertexShader* g_pVertexShader;
	ID3D11PixelShader* g_pPixelShader;
	// 着色器输入布局
	ID3D11InputLayout* g_pInputLayout;
	// 纹理资源
	ID3D11ShaderResourceView* g_pTexture1SRV;
	ID3D11ShaderResourceView* g_pTexture2SRV;
	ID3D11ShaderResourceView* g_pTexture3SRV;
	ID3D11ShaderResourceView* g_pTexture4SRV;
	ID3D11ShaderResourceView* g_pTexture5SRV;
	ID3D11ShaderResourceView* g_pTexture6SRV;
	// 保存的图片
	ID3D11Texture2D* pSavedTex;
	ID3D11Resource* pSavedRes;
	// 渲染目标视图
	ID3D11RenderTargetView* g_pRenderTargetView;
	// D3D11交换链
	IDXGISwapChain* g_pSwapChain;
	// 保存测试
	//ID3D11Texture2D* Saved;
};