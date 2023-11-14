#include "material.h"

void WaterMaterial::Start()
{
	SetFloat("_Time", 0.0f);
	SetFloat("_Frequency", 10.0f);

	SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::WATER_T));

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\unlitWaterVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\unlitWaterPS.cso");
}

void WaterMaterial::Update()
{
	floats["_Time"] += Time::deltaTime;

	//floats["_Frequency"] = sinf(floats["_Time"]);

}

void WaterMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveThreshold = floats["_Frequency"];
	param.dissolveRange = floats["_Time"];
	param.color = gameObject->GetColor();

	Renderer::SetParameter(param);
}