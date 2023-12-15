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

float a = 0.0f;
void GeometryInstancingMaterial::Draw()
{
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(positionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	D3DXVECTOR3* vertex = (D3DXVECTOR3*)msr.pData;

	int i = 0;
	for (int x = 0; x < 100; x++)
	{
		for (int z = 0; z < 100; z++)
		{
			a += 0.00001f;

			vertex[i].x = x * 40.0f;
			vertex[i].y = sinf(a) * 5.0f;
			vertex[i].z = z * 40.0f;
			i++;
		}
	}

	Renderer::GetDeviceContext()->Unmap(positionBuffer, 0);

	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
	if (textures["_Normal_Map"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &textures["_Normal_Map"]); }

}

void GeometryInstancingMaterial::CreatePositionBuffer()
{
	D3DXVECTOR3* pos = new D3DXVECTOR3[10000];

	int i = 0;
	for (int x = 0; x < 100; x++)
	{
		for (int z = 0; z < 100; z++)
		{
			pos[i] = D3DXVECTOR3(x, 0.0f, z);
			i++;
		}
	}

	//頂点バッファー生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(D3DXVECTOR3) * 10000;
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
	srvd.Buffer.NumElements = 10000;
	Renderer::GetDevice()->CreateShaderResourceView(positionBuffer, &srvd, &positionSRV);
}

void GeometryInstancingMaterial::GeometryInstancing()
{
	Renderer::GetDeviceContext()->VSSetShaderResources(5, 1, &positionSRV);
	Renderer::GetDeviceContext()->DrawInstanced(4, 10000, 0, 0);
}