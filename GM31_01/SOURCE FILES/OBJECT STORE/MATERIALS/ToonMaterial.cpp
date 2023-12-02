#include "material.h"

void ToonMaterial::Start()
{
	SetFloat("_Offset", 0.0f);

	SetTexture("_Texture", TextureReader::TOON_T);

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
	param.dissolveRange = floats["_Offset"];

	Renderer::SetParameter(param);
}