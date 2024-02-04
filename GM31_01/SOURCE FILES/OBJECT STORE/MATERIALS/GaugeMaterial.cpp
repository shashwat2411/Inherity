#include "material.h"

void GaugeMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	SetColor("_HP", D3DXCOLOR(1000.0f, 1000.0f, 1000.0f, 0.0f));
	SetColor("_Base_Color", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetColor("_Lost_Color", D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	SetColor("_Difference_Color", D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	SetFloat("_Thickness_X", 0.04f);
	SetFloat("_Thickness_Y", 0.04f);

	SetTexture("_Texture", TextureReader::BOX_T);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\gaugeVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\gaugePS.cso");
}

void GaugeMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.color = colors["_HP"];
	param.color2 = colors["_Base_Color"];
	param.color3= colors["_Lost_Color"];
	param.color4 = colors["_Difference_Color"];
	param.dummy[0] = floats["_Thickness_X"];
	param.dummy[1] = floats["_Thickness_Y"];

	Renderer::SetParameter(param);
}