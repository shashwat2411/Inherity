#include "material.h"

void SpriteMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	SetTexture("_Texture", nullptr);
	SetTexture("_Normal_Map", nullptr);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\unlitTexturePS.cso");
}

void SpriteMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
	if (textures["_Normal_Map"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

}