#include "material.h"

void MergeMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	SetTexture("_Texture", nullptr);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\mergeVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\mergePS.cso");
}

void MergeMaterial::Draw()
{
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Renderer::GetPostProcessTexture());
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, Renderer::GetLuminanceTexture());
	Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, Renderer::GetBloomTexture(0));
	Renderer::GetDeviceContext()->PSSetShaderResources(3, 1, Renderer::GetBloomTexture(1));
	Renderer::GetDeviceContext()->PSSetShaderResources(4, 1, Renderer::GetBloomTexture(2));
	Renderer::GetDeviceContext()->PSSetShaderResources(5, 1, Renderer::GetBloomTexture(3));
	Renderer::GetDeviceContext()->PSSetShaderResources(6, 1, Renderer::GetBloomTexture(4));
	Renderer::GetDeviceContext()->PSSetShaderResources(7, 1, Renderer::GetBloomTexture(5));
}