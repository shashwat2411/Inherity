#pragma once
#include "main.h"




// 頂点構造体
struct VERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 Tangent;
	D3DXVECTOR3 Binormal;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR2 TexCoord;
};



// マテリアル構造体
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};




// マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL		Material;
	class CTexture*	Texture;
};


// 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];//16byte境界用
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;

	D3DXMATRIX	viewMatrix;
	D3DXMATRIX	projectionMatrix;
};

struct PARAMETER
{
	float	dissolveThreshold;
	float	dissolveRange;
	float	dummy[2];
	D3DXCOLOR	color;
};


class VertexBuffer;
class IndexBuffer;
class Texture;

enum CULL_MODE
{
	CULL_MODE_NONE,			//カリング無し
	CULL_MODE_FRONT,		//表のポリゴンを描画しない(CW)
	CULL_MODE_BACK,			//裏のポリゴンを描画しない(CCW)

	CULL_MODE_NUM
};

enum SAMPLER_STATE
{
	SAMPLER_STATE_WRAP,
	SAMPLER_STATE_CLAMP,
	SAMPLER_STATE_MIRROR,

	SAMPLER_STATE_MAX
};

class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;


	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;
	static ID3D11Buffer*			m_CameraBuffer;
	static ID3D11Buffer*			m_ParameterBuffer;

	static ID3D11SamplerState*		m_samplerState_W;
	static ID3D11SamplerState*		m_samplerState_C;
	static ID3D11SamplerState*		m_samplerState_M;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState*		m_BlendState;
	static ID3D11BlendState*		m_BlendStateATC;

	static ID3D11DepthStencilView* m_DepthShadowDepthStencilView;
	static ID3D11ShaderResourceView* m_DepthShadowShaderResourceView;



public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetCullingMode(CULL_MODE cm);

	static void SetCameraPosition(D3DXVECTOR3 CameraPosition);
	static void SetParameter(PARAMETER param);
	static void SetSamplerState(SAMPLER_STATE sampler, int i = 0);

	static ID3D11Device* GetDevice(void) { return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }
	static ID3D11RenderTargetView** GetRendererTargetView(void) { return &m_RenderTargetView; }
	static ID3D11ShaderResourceView* GetDepthShadowTexture() { return m_DepthShadowShaderResourceView; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);


	static void BeginDepth()
	{
		m_DeviceContext->OMSetRenderTargets(0, NULL, m_DepthShadowDepthStencilView);
		m_DeviceContext->ClearDepthStencilView(m_DepthShadowDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	static void SetDefaultViewPort();
	static void SetDepthViewPort(float multiplier);
	static void SetDepthViewPort();
};
