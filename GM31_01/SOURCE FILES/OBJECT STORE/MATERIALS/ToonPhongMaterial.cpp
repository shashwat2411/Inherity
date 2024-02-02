#include "material.h"

void ToonPhongMaterial::Start()
{
	SetFloat("_Dissolve", -1.0f);

	SetFloat("_Threshold", 0.0f);
	SetFloat("_Dissolve_Range", 0.4f);

	SetColor("_Dissolve_Color", D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	textures["_Texture"] = nullptr;
	SetTexture("_Texture", TextureReader::WIPE_T);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\toonVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\toonPhongPS.cso");
}

void ToonPhongMaterial::Update()
{

}

void ToonPhongMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Texture"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveThreshold = (1.0f - floats["_Threshold"]);
	param.dissolveRange = floats["_Dissolve_Range"];
	param.color = colors["_Dissolve_Color"];
	param.color2.r = floats["_Dissolve"];

	Renderer::SetParameter(param);
}