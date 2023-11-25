
#include "main.h"
#include "renderer.h"
#include <io.h>

#define MULTI_SAMPLE_ANTI_ALIASING 8

D3D_FEATURE_LEVEL	Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*			Renderer::m_Device{};
ID3D11DeviceContext*	Renderer::m_DeviceContext{};

IDXGISwapChain*	Renderer::m_SwapChain{};

ID3D11RenderTargetView*	Renderer::m_RenderTargetView{};
ID3D11RenderTargetView*	Renderer::m_PostProcessRenderTargetView{};
ID3D11RenderTargetView*	Renderer::m_ReflectRenderTargetView{};

ID3D11DepthStencilView*	Renderer::m_DepthStencilView{};
ID3D11DepthStencilView*	Renderer::m_DepthShadowDepthStencilView{};
ID3D11DepthStencilView*	Renderer::m_ReflectDepthStencilView{};

ID3D11ShaderResourceView*	Renderer::m_DepthShadowShaderResourceView{};
ID3D11ShaderResourceView*	Renderer::m_PostProcessShaderResourceView{};
ID3D11ShaderResourceView*	Renderer::m_CubeReflectShaderResourceView{};

ID3D11Buffer*	Renderer::m_WorldBuffer{};
ID3D11Buffer*	Renderer::m_ViewBuffer{};
ID3D11Buffer*	Renderer::m_ProjectionBuffer{};
ID3D11Buffer*	Renderer::m_MaterialBuffer{};
ID3D11Buffer*	Renderer::m_LightBuffer{};
ID3D11Buffer*	Renderer::m_CameraBuffer = NULL;
ID3D11Buffer*	Renderer::m_ParameterBuffer = NULL;

ID3D11SamplerState*	Renderer::m_samplerState_W{};
ID3D11SamplerState*	Renderer::m_samplerState_C{};
ID3D11SamplerState*	Renderer::m_samplerState_M{};


ID3D11DepthStencilState* Renderer::m_DepthStateEnable{};
ID3D11DepthStencilState* Renderer::m_DepthStateDisable{};


ID3D11BlendState*	Renderer::m_BlendState{};
ID3D11BlendState*	Renderer::m_BlendStateATC{};


ID3D11Texture2D*	Renderer::m_ReflectTexture{};
ID3D11Texture2D*	Renderer::m_CubeReflectTexture{};


ID3D11RasterizerState*	g_RasterStateCullOff;
ID3D11RasterizerState*	g_RasterStateCullCW;
ID3D11RasterizerState*	g_RasterStateCullCCW;


void Renderer::Init()
{
	HRESULT hr = S_OK;




	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();

#ifdef MULTI_SAMPLE_ANTI_ALIASING
	swapChainDesc.SampleDesc.Count = MULTI_SAMPLE_ANTI_ALIASING;	//Multiple Anti Aliasing ( 8 ) // Default ( 1 )
#else
	swapChainDesc.SampleDesc.Count = 1;	//Multiple Anti Aliasing ( 8 ) // Default ( 1 )
#endif

	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_SwapChain,
		&m_Device,
		&m_FeatureLevel,
		&m_DeviceContext);






	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget{};
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
	renderTarget->Release();


	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = textureDesc.Format;

#ifdef MULTI_SAMPLE_ANTI_ALIASING
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; //MAA ( D3D11_DSV_DIMENSION_TEXTURE2DMS ) //Default ( D3D11_DSV_DIMENSION_TEXTURE2D )
#else
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; //MAA ( D3D11_DSV_DIMENSION_TEXTURE2DMS ) //Default ( D3D11_DSV_DIMENSION_TEXTURE2D )
#endif

	depthStencilViewDesc.Flags = 0;
	m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	depthStencile->Release();


	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);


	// ビューポート設定
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)SCREEN_WIDTH;
	viewport.Height = (FLOAT)SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &viewport);



	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	ID3D11RasterizerState *rs;
	m_Device->CreateRasterizerState(&rasterizerDesc, &rs);

	m_DeviceContext->RSSetState(rs);

	//m_Device->CreateRasterizerState(&rasterizerDesc, &g_RasterStateCullOff);

	//rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	//m_Device->CreateRasterizerState(&rasterizerDesc, &g_RasterStateCullCW);

	//rasterizerDesc.CullMode = D3D11_CULL_BACK;
	//m_Device->CreateRasterizerState(&rasterizerDesc, &g_RasterStateCullCCW);

	// カリングモード設定（CCW）
	//SetCullingMode(CULL_MODE_BACK);


	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc{};
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_Device->CreateBlendState(&blendDesc, &m_BlendState);

	blendDesc.AlphaToCoverageEnable = TRUE;
	m_Device->CreateBlendState(&blendDesc, &m_BlendStateATC);

	m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);



	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//深度無効ステート

	m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);




	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc_W{};
	ZeroMemory(&samplerDesc_W, sizeof(samplerDesc_W));
	samplerDesc_W.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc_W.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // CLAMP , MIRROR
	samplerDesc_W.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc_W.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc_W.MipLODBias = 0;
	samplerDesc_W.MaxAnisotropy = 16;
	samplerDesc_W.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc_W.MinLOD = 0;
	samplerDesc_W.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc_W, &m_samplerState_W);

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc_C{};
	ZeroMemory(&samplerDesc_C, sizeof(samplerDesc_C));
	samplerDesc_C.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc_C.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP; // CLAMP , MIRROR
	samplerDesc_C.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc_C.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc_C.MipLODBias = 0;
	samplerDesc_C.MaxAnisotropy = 16;
	samplerDesc_C.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc_C.MinLOD = 0;
	samplerDesc_C.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc_C, &m_samplerState_C);

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc_M{};
	ZeroMemory(&samplerDesc_M, sizeof(samplerDesc_M));
	samplerDesc_M.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc_M.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR; // CLAMP , MIRROR
	samplerDesc_M.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc_M.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc_M.MipLODBias = 0;
	samplerDesc_M.MaxAnisotropy = 16;
	samplerDesc_M.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc_M.MinLOD = 0;
	samplerDesc_M.MaxLOD = D3D11_FLOAT32_MAX;

	m_Device->CreateSamplerState(&samplerDesc_M, &m_samplerState_M);

	m_DeviceContext->PSSetSamplers(0, 1, &m_samplerState_W);
	m_DeviceContext->PSSetSamplers(1, 1, &m_samplerState_C);
	//こいつを3つ分用意して、描画のところに切り替えてる

	//Shadow
	{
		swapChainDesc.SampleDesc.Count = 1;

		//シャドーバッファー作成
		ID3D11Texture2D* depthTexture = NULL;
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(&td));
		td.Width = 4096;
		td.Height = 4096;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R32_TYPELESS;	//32bit の自由な形式のデータとする
		td.SampleDesc = swapChainDesc.SampleDesc;//
		td.Usage = D3D11_USAGE_DEFAULT;	//	↓デプス＆ステンシルバッファとして作成
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		m_Device->CreateTexture2D(&td, NULL, &depthTexture);

		//デプスステンシルターゲットビュー作成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;	//ピクセルフォーマットは 32	BitFLOAT型
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		m_Device->CreateDepthStencilView(depthTexture, &dsvd, &m_DepthShadowDepthStencilView);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R32_FLOAT;	//ピクセルフォーマットは 32BitFLOAT型
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(depthTexture, &srvd, &m_DepthShadowShaderResourceView);

		depthTexture->Release();

		swapChainDesc.SampleDesc.Count = MULTI_SAMPLE_ANTI_ALIASING;
	}

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_WorldBuffer);
	m_DeviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ViewBuffer);
	m_DeviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ProjectionBuffer);
	m_DeviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);
	m_DeviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);
	m_DeviceContext->PSSetConstantBuffers(3, 1, &m_MaterialBuffer);


	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_LightBuffer);
	m_DeviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
	m_DeviceContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);
	bufferDesc.ByteWidth = sizeof(D3DXVECTOR4);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_CameraBuffer);
	m_DeviceContext->PSSetConstantBuffers(5, 1, &m_CameraBuffer);


	bufferDesc.ByteWidth = sizeof(PARAMETER);

	m_Device->CreateBuffer(&bufferDesc, NULL, &m_ParameterBuffer);
	m_DeviceContext->VSSetConstantBuffers(6, 1, &m_ParameterBuffer);
	m_DeviceContext->PSSetConstantBuffers(6, 1, &m_ParameterBuffer);



	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	//light.Direction = D3DXVECTOR4(0.5f, -1.0f, 0.8f, 0.0f);
	//D3DXVec4Normalize(&light.Direction, &light.Direction);
	//light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	//light.Diffuse = D3DXCOLOR(1.5f, 1.5f, 1.5f, 1.0f);
	SetLight(light);



	// マテリアル初期化
	MATERIAL material{};
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	//Reflect Rendering
	{
		{
			swapChainDesc.BufferCount = 1;
			swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
			swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow = GetWindow();
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.Windowed = TRUE;

			D3D11_TEXTURE2D_DESC td;
			ZeroMemory(&td, sizeof(td));
			td.ArraySize = 1;
			td.Width = 512;
			td.Height = 512;
			td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			td.Usage = D3D11_USAGE_DEFAULT;
			td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			td.SampleDesc = swapChainDesc.SampleDesc;
			td.MiscFlags = 0;
			td.MipLevels = 1;
			m_Device->CreateTexture2D(&td, NULL, &m_ReflectTexture);


			D3D11_RENDER_TARGET_VIEW_DESC rtvd;
			ZeroMemory(&rtvd, sizeof(rtvd));
			rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			m_Device->CreateRenderTargetView(m_ReflectTexture, &rtvd, &m_ReflectRenderTargetView);

			ID3D11Texture2D* depthTexture = NULL;
			ZeroMemory(&td, sizeof(td));
			td.Width = 512;
			td.Height = 512;
			td.MipLevels = 1;
			td.ArraySize = 1;
			td.Format = DXGI_FORMAT_D32_FLOAT;
			td.SampleDesc.Count = 1;
			td.SampleDesc.Quality = 0;
			td.Usage = D3D11_USAGE_DEFAULT;
			td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			td.CPUAccessFlags = 0;
			td.MiscFlags = 0;
			m_Device->CreateTexture2D(&td, NULL, &depthTexture);


			D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
			ZeroMemory(&dsvd, sizeof(dsvd));
			dsvd.Format = DXGI_FORMAT_D32_FLOAT;
			dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

			m_Device->CreateDepthStencilView(depthTexture, &dsvd, &m_ReflectDepthStencilView);//深度有効ステート
			depthTexture->Release();
		}

		{
			D3D11_TEXTURE2D_DESC td;
			ZeroMemory(&td, sizeof(td));
			td.ArraySize = 6;
			td.Width = 512;
			td.Height = 512;
			td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			td.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;
			td.MipLevels = 1;
			td.Usage = D3D11_USAGE_DEFAULT;
			td.SampleDesc = swapChainDesc.SampleDesc;
			m_Device->CreateTexture2D(&td, NULL, &m_CubeReflectTexture);

			D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
			ZeroMemory(&srvd, sizeof(srvd));
			srvd.Format = td.Format;
			srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvd.TextureCube.MipLevels = td.MipLevels;
			srvd.TextureCube.MostDetailedMip = 0;
			m_Device->CreateShaderResourceView(m_CubeReflectTexture, &srvd, &m_CubeReflectShaderResourceView);
		}

		{
			D3D11_TEXTURE2D_DESC td;
			ZeroMemory(&td, sizeof(td));
			td.ArraySize = 6;
			td.Width = 512;
			td.Height = 512;
			td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			td.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;
			td.MipLevels = 1;
			td.Usage = D3D11_USAGE_DEFAULT;
			td.SampleDesc = swapChainDesc.SampleDesc;
			m_Device->CreateTexture2D(&td, NULL, &m_CubeReflectTexture);

			D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
			ZeroMemory(&srvd, sizeof(srvd));
			srvd.Format = td.Format;
			srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvd.TextureCube.MipLevels = td.MipLevels;
			srvd.TextureCube.MostDetailedMip = 0;
			m_Device->CreateShaderResourceView(m_CubeReflectTexture, &srvd, &m_CubeReflectShaderResourceView);
		}
	}

	//Post Process
	{
		swapChainDesc.SampleDesc.Count = 1;

		//PPテクスチャ作成
		ID3D11Texture2D* ppTexture = NULL;
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc = swapChainDesc.SampleDesc;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;
		m_Device->CreateTexture2D(&td, NULL, &ppTexture);

		D3D11_RENDER_TARGET_VIEW_DESC rtvd;
		ZeroMemory(&rtvd, sizeof(rtvd));
		rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		m_Device->CreateRenderTargetView(ppTexture, &rtvd, &m_PostProcessRenderTargetView);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
		ZeroMemory(&srvd, sizeof(srvd));
		srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		m_Device->CreateShaderResourceView(ppTexture, &srvd, &m_PostProcessShaderResourceView);

		ppTexture->Release();
	}
}



void Renderer::Uninit()
{
	if (g_RasterStateCullOff)	g_RasterStateCullOff->Release();
	if (g_RasterStateCullCW)	g_RasterStateCullCW->Release();
	if (g_RasterStateCullCCW)	g_RasterStateCullCCW->Release();

	//Buffer
	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();

	m_DeviceContext->ClearState();
	
	m_SwapChain->Release();
	
	m_DeviceContext->Release();
	m_Device->Release();
}




void Renderer::Begin()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	// バックバッファクリア
	float clearColor[4] = { 1.0f, 0.0f, 0.5f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

//void Renderer::BeginPostProcess()
//{
//	m_DeviceContext->OMSetRenderTargets(1, &m_PostProcessRenderTargetView, m_DepthStencilView);
//
//	float clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
//	m_DeviceContext->ClearRenderTargetView(m_PostProcessRenderTargetView, clearColor);
//	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
//}

void Renderer::BeginCube()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_ReflectRenderTargetView, m_ReflectDepthStencilView);

	// バックバッファクリア
	float clearColor[4] = { 1.0f, 0.0f, 0.5f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_ReflectRenderTargetView, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_ReflectDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}



void Renderer::End()
{
	m_SwapChain->Present(1, 0);
}




void Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
	else
		m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);

}



void Renderer::SetATCEnable(bool Enable)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

}

void Renderer::SetWorldViewProjection2D()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);

}


void Renderer::SetWorldMatrix(D3DXMATRIX* WorldMatrix)
{
	D3DXMATRIX world;
	D3DXMatrixTranspose(&world, WorldMatrix);
	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetViewMatrix(D3DXMATRIX* ViewMatrix)
{
	D3DXMATRIX view;
	D3DXMatrixTranspose(&view, ViewMatrix);
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

void Renderer::SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix)
{
	D3DXMATRIX projection;
	D3DXMatrixTranspose(&projection, ProjectionMatrix);
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}



void Renderer::SetMaterial(MATERIAL Material)
{
	m_DeviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);
}

void Renderer::SetLight(LIGHT Light)
{
	//シェーダー側の都合上で行列を転置しておく
	D3DXMatrixTranspose(&Light.viewMatrix, &Light.viewMatrix);
	D3DXMatrixTranspose(&Light.projectionMatrix, &Light.projectionMatrix);

	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetCullingMode(CULL_MODE cm)
{
	switch (cm)
	{
	case CULL_MODE_NONE:
		m_DeviceContext->RSSetState(g_RasterStateCullOff);
		break;
	case CULL_MODE_FRONT:
		m_DeviceContext->RSSetState(g_RasterStateCullCW);
		break;
	case CULL_MODE_BACK:
		m_DeviceContext->RSSetState(g_RasterStateCullCCW);
		break;
	}
}

void Renderer::SetCameraPosition(D3DXVECTOR3 CameraPosition)
{
	D3DXVECTOR4 cpos(CameraPosition.x, CameraPosition.y, CameraPosition.z, 1.0f);
	m_DeviceContext->UpdateSubresource(m_CameraBuffer, 0, NULL, &cpos, 0, 0);
}

void Renderer::SetParameter(PARAMETER param)
{
	m_DeviceContext->UpdateSubresource(m_ParameterBuffer, 0, NULL, &param, 0, 0);
}

void Renderer::SetSamplerState(SAMPLER_STATE sampler, int i)
{
	switch (sampler)
	{
	case SAMPLER_STATE_WRAP:
		m_DeviceContext->PSSetSamplers(i, 1, &m_samplerState_W);
		break;
	case SAMPLER_STATE_CLAMP:
		m_DeviceContext->PSSetSamplers(i, 1, &m_samplerState_C);
		break;
	case SAMPLER_STATE_MIRROR:
		m_DeviceContext->PSSetSamplers(i, 1, &m_samplerState_M);
		break;
	}
}


void Renderer::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName)
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 9, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}

void Renderer::SetDefaultViewPort()
{
	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_DeviceContext->RSSetViewports(1, &vp);
}

void Renderer::SetDepthViewPort()
{
	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = 4096;
	vp.Height = 4096;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_DeviceContext->RSSetViewports(1, &vp);
}

void Renderer::SetDepthViewPort(float multiplier)
{
	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH * multiplier;
	vp.Height = (FLOAT)SCREEN_HEIGHT * multiplier;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_DeviceContext->RSSetViewports(1, &vp);
}

void Renderer::SetReflectViewPort()
{
	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = 512;
	vp.Height = 512;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	m_DeviceContext->RSSetViewports(1, &vp);
}