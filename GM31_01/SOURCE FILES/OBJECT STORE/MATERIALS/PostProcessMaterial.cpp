#include "material.h"

void PostProcessMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\postNoiseVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\postNoisePS.cso");
}

void PostProcessMaterial::Draw()
{
	ID3D11ShaderResourceView* ppTexture = Renderer::GetPostProcessTexture();
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &ppTexture);
}