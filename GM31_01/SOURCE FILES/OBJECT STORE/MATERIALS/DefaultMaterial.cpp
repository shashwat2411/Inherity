#include "material.h"

void DefaultMaterial::Start()
{
	//gameObject->SetDepthShadow(true);

	SetTexture("_Texture", nullptr);
	SetTexture("_Normal_Map", nullptr);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\vertexLightingPS.cso");
}

void DefaultMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
	if (textures["_Normal_Map"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveRange = (textures["_Normal_Map"] != nullptr ? 2.0f : 0.0f);
	param.color = gameObject->GetColor();

	Renderer::SetParameter(param);
}