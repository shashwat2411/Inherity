#include "material.h"

void GaugeMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	SetColor("_HP", D3DXCOLOR(200.0f, 100.0f, 200.0f, 2.0f));
	SetColor("_Base_Color", D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	SetColor("_Lost_Color", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	SetColor("_Difference_Color", D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	SetTexture("_Texture", nullptr);
	SetTexture("_Normal_Map", nullptr);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\gaugeVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\gaugePS.cso");
}

void GaugeMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
	if (textures["_Normal_Map"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.color = colors["_HP"];
	param.color2 = colors["_Base_Color"];
	param.color3= colors["_Lost_Color"];
	param.color4 = colors["_Difference_Color"];

	Renderer::SetParameter(param);
}