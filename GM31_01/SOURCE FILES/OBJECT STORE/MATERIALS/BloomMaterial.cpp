#include "material.h"

void BloomMaterial::Start()
{
	gameObject->SetDepthShadow(false);

	SetInt("_Index", 0);

	SetTexture("_Texture", nullptr);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\bloomVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\bloomPS.cso");
}

void BloomMaterial::Draw()
{
	if (ints["_Index"] <= 0 || ints["_Index"] >= 4) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Renderer::GetLuminanceTexture()); }
	else { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, Renderer::GetBloomTexture(ints["_Index"] - 1)); }

	PARAMETER p;
	p.dummy[0] = 1.0f / (ints["_Index"] + 1);

	if (ints["_Index"] > 0 && ints["_Index"] < 4)
	{
		p.color = D3DXCOLOR(-3, 3, -3, 3);
		p.color2 = D3DXCOLOR(3.0f, 3.5f, 0.7f, 1.0f);
	}
	else if(ints["_Index"] == 4)
	{
		p.color = D3DXCOLOR(0, 1, -32, 32);
		p.color2 = D3DXCOLOR(1.0f, 1.0f, 0.3f, 4.0f);
	}
	else if (ints["_Index"] == 5)
	{
		p.color = D3DXCOLOR(-32, 32, 0, 1);
		p.color2 = D3DXCOLOR(1.0f, 1.0f, 0.7f, 4.0f);
	}
	Renderer::SetParameter(p);
}