#include "material.h"

void DefaultMaterial::Start()
{
	gameObject->SetDepthShadow(true);

	SetTexture("_Texture", /*TextureReader::GetReadTexture(TextureReader::BOX_T)*/nullptr);
	SetTexture("_Normal_Map", nullptr);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\vertexLightingPS.cso");
}

void DefaultMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
	if (textures["_Normal_Map"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

}