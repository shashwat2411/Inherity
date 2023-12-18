#include "material.h"	

void GeometryInstancingMaterial::Start()
{
	positionBuffer = nullptr;
	positionSRV = nullptr;

	gameObject->SetDepthShadow(false);
	gameObject->SetBillboard(false);
	gameObject->SetGeometri(true);

	SetTexture("_Texture", nullptr);
	SetTexture("_Normal_Map", nullptr);

	CreatePositionBuffer();

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\geometryInstancingVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\unlitTexturePS.cso");
}

void GeometryInstancingMaterial::End()
{
	if (positionBuffer != nullptr) { positionBuffer->Release(); }
	if (positionSRV != nullptr) { positionSRV->Release(); }
}
void GeometryInstancingMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
	if (textures["_Normal_Map"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

}

void GeometryInstancingMaterial::CreatePositionBuffer()
{
	D3DXVECTOR3* pos = new D3DXVECTOR3[MAX_PARTICLES];

	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		pos[x] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//頂点バッファー生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(D3DXVECTOR3) * MAX_PARTICLES;
	bd.StructureByteStride = sizeof(D3DXVECTOR3);
	bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = pos;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &positionBuffer);

	delete[] pos;

	//シェーダーリソースビュー生成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvd.Buffer.FirstElement = 0;
	srvd.Buffer.NumElements = MAX_PARTICLES;
	Renderer::GetDevice()->CreateShaderResourceView(positionBuffer, &srvd, &positionSRV);
}

void GeometryInstancingMaterial::GeometryInstancing()
{
	Renderer::GetDeviceContext()->VSSetShaderResources(5, 1, &positionSRV);
	Renderer::GetDeviceContext()->DrawInstanced(4, MAX_PARTICLES, 0, 0);
}