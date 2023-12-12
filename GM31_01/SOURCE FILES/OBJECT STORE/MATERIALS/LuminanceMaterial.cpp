#include "material.h"

void LuminanceMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	SetTexture("_Texture", nullptr);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\luminanceVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\luminancePS.cso");
}

void LuminanceMaterial::Draw()
{
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Renderer::GetPostProcessTexture());
}