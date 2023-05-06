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
	HINSTANCE m_hAppInst;        // Ӧ��ʵ�����
	HWND      m_hMainWnd;        // �����ھ��

	// Ӧ�ó��򴰿ڵĿ�Ⱥ͸߶�
	int g_WindowWidth = 7680;
	int g_WindowHeight = 3840;
	// Direct3D 11 ���豸���豸������
	ID3D11Device* g_pd3dDevice;
	ID3D11DeviceContext* g_pd3dDeviceContext;
	// ������Ⱦ�Ķ��������������
	ID3D11Buffer* g_pVertexBuffer;
	ID3D11Buffer* g_pIndexBuffer;
	// ������ɫ����������ɫ��
	ID3D11VertexShader* g_pVertexShader;
	ID3D11PixelShader* g_pPixelShader;
	// ��ɫ�����벼��
	ID3D11InputLayout* g_pInputLayout;
	// ������Դ
	ID3D11ShaderResourceView* g_pTexture1SRV;
	ID3D11ShaderResourceView* g_pTexture2SRV;
	ID3D11ShaderResourceView* g_pTexture3SRV;
	ID3D11ShaderResourceView* g_pTexture4SRV;
	ID3D11ShaderResourceView* g_pTexture5SRV;
	ID3D11ShaderResourceView* g_pTexture6SRV;
	// �����ͼƬ
	ID3D11Texture2D* pSavedTex;
	ID3D11Resource* pSavedRes;
	// ��ȾĿ����ͼ
	ID3D11RenderTargetView* g_pRenderTargetView;
	// D3D11������
	IDXGISwapChain* g_pSwapChain;
	// �������
	//ID3D11Texture2D* Saved;
};