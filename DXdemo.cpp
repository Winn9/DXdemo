#include "DXdemo.h"

#include <cstdio>

ID3D11Texture2D* Saved = nullptr;

DXdemo::DXdemo(HINSTANCE hInstance, int initWidth, int initHeight):
	m_hAppInst(hInstance),
	m_hMainWnd(nullptr),
	g_WindowWidth(initWidth),
	g_WindowHeight(initHeight),
	g_pd3dDevice(nullptr),
	g_pd3dDeviceContext(nullptr),
	g_pVertexBuffer(nullptr),
	g_pIndexBuffer(nullptr),
	g_pVertexShader(nullptr),
	g_pPixelShader(nullptr),
	g_pInputLayout(nullptr),
	g_pTexture1SRV(nullptr),
	g_pTexture2SRV(nullptr),
	g_pTexture3SRV(nullptr),
	g_pTexture4SRV(nullptr),
	g_pTexture5SRV(nullptr),
	g_pTexture6SRV(nullptr),
	pSavedTex(nullptr),
	pSavedRes(nullptr),
	g_pRenderTargetView(nullptr),
	g_pSwapChain(nullptr)
{
}

DXdemo::~DXdemo()
{
	Cleanup();
}

bool DXdemo::Init(HWND m_hMainWnd, int g_WindowWidth, int g_WindowHeight)
{
	if (!InitD3D(m_hMainWnd, g_WindowWidth, g_WindowHeight))
		return false;
	if (!InitEffect())
		return false;
	if (!InitResource())
		return false;

	return true;
}

bool DXdemo::InitD3D(HWND m_hMainWnd, int g_WindowWidth, int g_WindowHeight)
{
    // 创建 Direct3D 11 设备和设备上下文
    D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &g_pd3dDevice, nullptr, &g_pd3dDeviceContext);
   
    if (g_pd3dDeviceContext == nullptr)
        return false;

    // 初始化交换链
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(scd));
    scd.BufferCount = 1;
    scd.BufferDesc.Width = g_WindowWidth;
    scd.BufferDesc.Height = g_WindowHeight;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = m_hMainWnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;
    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &g_pSwapChain, &g_pd3dDevice, nullptr, &g_pd3dDeviceContext);


    // 设置渲染目标视图
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = 7680;
    textureDesc.Height = 3840;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    ID3D11Texture2D* pTexture = nullptr;
    g_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);

    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    if (pBackBuffer != nullptr) {
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
    }
    else {
        return false;
    }
	Saved = pBackBuffer;
	D3DX11SaveTextureToFile(g_pd3dDeviceContext, Saved, D3DX11_IFF_PNG, "D:\\data\\Dx\\TargetTexture1.png");
    pBackBuffer->Release();
    // 将渲染目标视图结合到管线
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_pRenderTargetView, nullptr);

    // 设置视口
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)g_WindowWidth;
    vp.Height = (FLOAT)g_WindowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pd3dDeviceContext->RSSetViewports(1, &vp);
}

bool DXdemo::InitEffect()
{
    // 创建和编译顶点着色器和像素着色器
    ID3DBlob* pVSBlob = nullptr;
    D3DX11CompileFromFile("VertexShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, nullptr, &pVSBlob, nullptr, nullptr);
    g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);

    ID3DBlob* pPSBlob = nullptr;
    D3DX11CompileFromFile("PixelShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, nullptr, &pPSBlob, nullptr, nullptr);
    g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);

    // 设置着色器输入布局
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    g_pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pInputLayout);

    return true;
}

bool DXdemo::InitResource()
{
    // 创建顶点和索引
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
    };

    Vertex vertices[] =
    {
        { DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
        { DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
        { DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
        { DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
    };

    unsigned short indices[] =
    {
        0, 1, 2,
        2, 1, 3
    };

    // 创建纹理资源
    D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, "D:\\data\\InternalRT\\InternalRTs[0].png", nullptr, nullptr, &g_pTexture1SRV, nullptr);
    D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, "D:\\data\\InternalRT\\InternalRTs[1].png", nullptr, nullptr, &g_pTexture2SRV, nullptr);
    D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, "D:\\data\\InternalRT\\InternalRTs[2].png", nullptr, nullptr, &g_pTexture3SRV, nullptr);
    D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, "D:\\data\\InternalRT\\InternalRTs[3].png", nullptr, nullptr, &g_pTexture4SRV, nullptr);
    D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, "D:\\data\\InternalRT\\InternalRTs[4].png", nullptr, nullptr, &g_pTexture5SRV, nullptr);
    D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, "D:\\data\\InternalRT\\InternalRTs[5].png", nullptr, nullptr, &g_pTexture6SRV, nullptr);

    // 初始化顶点缓冲区
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;
    // 创建顶点缓冲区
    D3D11_SUBRESOURCE_DATA vertexBufferData = {};
    vertexBufferData.pSysMem = vertices;
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;
    g_pd3dDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &g_pVertexBuffer);

    // 初始化索引缓冲区
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.ByteWidth = sizeof(unsigned short) * ARRAYSIZE(indices);
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;
    // 创建索引缓冲区
    D3D11_SUBRESOURCE_DATA indexBufferData = {};
    indexBufferData.pSysMem = indices;
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;
    g_pd3dDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, &g_pIndexBuffer);


    // 设置顶点缓冲区和索引缓冲区
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_pd3dDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
    g_pd3dDeviceContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // 设置图元类型和拓扑结构
    g_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 设置着色器资源
    g_pd3dDeviceContext->PSSetShaderResources(0, 1, &g_pTexture1SRV);
    g_pd3dDeviceContext->PSSetShaderResources(1, 1, &g_pTexture2SRV);
    g_pd3dDeviceContext->PSSetShaderResources(2, 1, &g_pTexture3SRV);
    g_pd3dDeviceContext->PSSetShaderResources(3, 1, &g_pTexture4SRV);
    g_pd3dDeviceContext->PSSetShaderResources(4, 1, &g_pTexture5SRV);
    g_pd3dDeviceContext->PSSetShaderResources(5, 1, &g_pTexture6SRV);

    // 设置着色器
    g_pd3dDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
    g_pd3dDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);

    // 设置着色器输入布局
    g_pd3dDeviceContext->IASetInputLayout(g_pInputLayout);

    return true;
}

void DXdemo::Render()
{
    // 清除渲染目标视图
    static float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };	// RGBA = (0,0,255,255)
    g_pd3dDeviceContext->ClearRenderTargetView(g_pRenderTargetView, blue);
	
	D3DX11SaveTextureToFile(g_pd3dDeviceContext, Saved, D3DX11_IFF_PNG, "D:\\data\\Dx\\TargetTexture2.png");
    // 绘制场景
    g_pd3dDeviceContext->DrawIndexed(6, 0, 0);
	D3DX11SaveTextureToFile(g_pd3dDeviceContext, Saved, D3DX11_IFF_PNG, "D:\\data\\Dx\\TargetTexture3.png");
    // 显示画面
    g_pSwapChain->Present(0, 0);
}

void DXdemo::Cleanup()
{
    if (g_pRenderTargetView) g_pRenderTargetView->Release();
    if (g_pVertexBuffer) g_pVertexBuffer->Release();
    if (g_pIndexBuffer) g_pIndexBuffer->Release();
    if (g_pVertexShader) g_pVertexShader->Release();
    if (g_pPixelShader) g_pPixelShader->Release();
    if (g_pInputLayout) g_pInputLayout->Release();
    if (pSavedTex) pSavedTex->Release();
    if (pSavedRes) pSavedRes->Release();
    if (g_pTexture1SRV) g_pTexture1SRV->Release();
    if (g_pTexture2SRV) g_pTexture2SRV->Release();
    if (g_pTexture3SRV) g_pTexture3SRV->Release();
    if (g_pTexture4SRV) g_pTexture4SRV->Release();
    if (g_pTexture5SRV) g_pTexture5SRV->Release();
    if (g_pTexture6SRV) g_pTexture6SRV->Release();
    if (g_pSwapChain) g_pSwapChain->Release();
    if (g_pd3dDeviceContext) g_pd3dDeviceContext->Release();
    if (g_pd3dDevice) g_pd3dDevice->Release();
}