#include "material.h"

void DrunkMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	SetFloat("_Time", 0.0f);

	SetTexture("_Texture", nullptr);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\drunkVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\drunkPS.cso");
}

void DrunkMaterial::Update()
{
	floats["_Time"] += Time::deltaTime;
}

void DrunkMaterial::Draw()
{
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Renderer::GetPostProcessTexture());

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.dissolveRange = floats["_Time"];


	Renderer::SetParameter(param);
}