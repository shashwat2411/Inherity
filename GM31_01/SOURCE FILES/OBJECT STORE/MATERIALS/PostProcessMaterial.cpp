#include "material.h"

void PostProcessMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\postMosaicVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\postMosaicPS.cso");
}

void PostProcessMaterial::Draw()
{
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Renderer::GetPostProcessTexture());
}