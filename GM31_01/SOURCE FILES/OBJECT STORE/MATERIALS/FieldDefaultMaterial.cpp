#include "material.h"

void FieldDefaultMaterial::Start()
{
	gameObject->SetDepthShadow(true);

	SetFloat("_Time", 0.0f);
	SetFloat("_Frequency", 10.0f);

	SetTexture("_Texture", TextureReader::BOX_T);
	SetTexture("_ToonTexture", TextureReader::TOON_T);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\DepthShadowMappingVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\DepthShadowMappingPS.cso");
}

void FieldDefaultMaterial::Update()
{
	//floats["_Time"] += Time::deltaTime;

	//floats["_Frequency"] = sinf(floats["_Time"]);

}

void FieldDefaultMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
	{ Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, Renderer::GetDepthShadowTexture()); }
	if (textures["_ToonTexture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(2, 1, &textures["_ToonTexture"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveThreshold = floats["_Frequency"];
	param.dissolveRange = 0.0f;
	param.color = gameObject->GetColor();

	Renderer::SetParameter(param);
}