#include "material.h"

void ToonMaterial::Start()
{
	SetFloat("_Dissolve", -1.0f);

	SetFloat("_Threshold", 2.0f);
	SetFloat("_Dissolve_Range", 0.4f);

	SetColor("_Dissolve_Color", D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	SetTexture("_Texture", TextureReader::DISSOLVE_T);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\toonVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\toonPS.cso");
}

void ToonMaterial::Update()
{
	//floats["_Offset"] += 0.01f;
}

void ToonMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Texture"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveThreshold = (1.0f - floats["_Threshold"]);
	param.dissolveRange = floats["_Dissolve_Range"];
	param.color = colors["_Dissolve_Color"];
	param.color2.r = floats["_Dissolve"];
	param.color3 = gameObject->GetColor();

	Renderer::SetParameter(param);
}