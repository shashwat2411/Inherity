#include "material.h"

void UnlitMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	SetTexture("_Texture", nullptr);
	SetTexture("_Normal_Map", nullptr);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\unlitDefaultVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\unlitDefaultPS.cso");
}

void UnlitMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
	if (textures["_Normal_Map"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveThreshold = 0.0f;
	param.dissolveRange = 0.0f;
	param.color = gameObject->GetColor();

	Renderer::SetParameter(param);
}