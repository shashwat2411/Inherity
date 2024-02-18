#include "material.h"

void WipeMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	SetFloat("_Threshold", 1.0f);

	SetTexture("_Texture", TextureReader::BOX_T);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\wipePS.cso");
}

void WipeMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveThreshold = (floats["_Threshold"]);

	Renderer::SetParameter(param);
}