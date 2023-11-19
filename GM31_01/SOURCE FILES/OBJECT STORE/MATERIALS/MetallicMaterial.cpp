#include "material.h"

void MetallicMaterial::Start()
{
	SetFloat("_Offset", 0.0f);

	SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::ENVMAP_T));
	SetTexture("_CubeTexture", TextureReader::GetReadTexture(TextureReader::ENVCUBEMAP_T));

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\envMappingVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\envMappingPS.cso");
}

void MetallicMaterial::Update()
{

}

void MetallicMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Texture"]); }
	if (textures["_CubeTexture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &textures["_CubeTexture"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveRange = floats["_Offset"];

	Renderer::SetParameter(param);
}