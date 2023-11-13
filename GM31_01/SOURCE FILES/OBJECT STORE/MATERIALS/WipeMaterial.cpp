#include "material.h"

void WipeMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	SetFloat("_Threshold", 1.0f);
	SetFloat("_Delta", 0.03f);

	SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::BOX_T));
	SetTexture("_Wipe_Texture", TextureReader::GetReadTexture(TextureReader::WIPE_T));

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\wipePS.cso");
}

void WipeMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
	if (textures["_Wipe_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Wipe_Texture"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveThreshold = (1.0f - floats["_Threshold"]);
	param.dissolveRange = 0.3f;
	param.color = gameObject->GetColor();

	Renderer::SetParameter(param);
}