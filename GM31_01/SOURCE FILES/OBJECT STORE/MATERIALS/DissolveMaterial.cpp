#include "material.h"

void DissolveMaterial::Start()
{
	gameObject->SetDepthShadow(true);

	SetFloat("_Threshold", 1.0f);
	SetFloat("_Delta", 0.0075f);
	SetFloat("_Dissolve_Range", 0.3f);

	SetTexture("_Dissolve_Texture", TextureReader::DISSOLVE_T);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\DepthShadowMappingVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\DissolveDepthShadowPS.cso");

}

void DissolveMaterial::Update()
{
	//if (floats["_Threshold"] > 1.1f || floats["_Threshold"] < 0.0f) { floats["_Delta"] *= -1.0f; }
	//floats["_Threshold"] += floats["_Delta"] * Time::fixedTimeScale;
}

void DissolveMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
	if (textures["_Dissolve_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Dissolve_Texture"]); }
	{ Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, Renderer::GetDepthShadowTexture()); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveThreshold = (1.0f - floats["_Threshold"]);
	param.dissolveRange = floats["_Dissolve_Range"];
	param.color = gameObject->GetColor();

	Renderer::SetParameter(param);
}