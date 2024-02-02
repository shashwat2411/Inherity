#include "material.h"	

void GeometryInstancingMaterial::Start()
{
	scaleBuffer = nullptr;
	rotationBuffer = nullptr;
	positionBuffer = nullptr;
	scaleSRV = nullptr;
	rotationSRV = nullptr;
	positionSRV = nullptr;

	gameObject->SetDepthShadow(false);
	gameObject->SetBillboard(false);
	gameObject->SetGeometry(true);

	CreateVertexBuffer(&scaleBuffer, &scaleSRV);
	CreateVertexBuffer(&rotationBuffer, &rotationSRV);
	CreateVertexBuffer(&positionBuffer, &positionSRV);

	SetTexture("_Texture", nullptr);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\geometryInstancingVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\unlitTexturePS.cso");
}

void GeometryInstancingMaterial::End()
{
	if (scaleBuffer != nullptr) { scaleBuffer->Release(); }
	if (rotationBuffer != nullptr) { rotationBuffer->Release(); }
	if (positionBuffer != nullptr) { positionBuffer->Release(); }
	if (scaleSRV != nullptr) { scaleSRV->Release(); }
	if (rotationSRV != nullptr) { rotationSRV->Release(); }
	if (positionSRV != nullptr) { positionSRV->Release(); }
}
void GeometryInstancingMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }
}

void GeometryInstancingMaterial::CreateVertexBuffer(ID3D11Buffer** buffer, ID3D11ShaderResourceView** srv)
{
	D3DXVECTOR3* value = new D3DXVECTOR3[MAX_PARTICLES];

	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		value[x] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
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
	sd.pSysMem = value;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, buffer);

	delete[] value;

	//シェーダーリソースビュー生成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = DXGI_FORMAT_UNKNOWN;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvd.Buffer.FirstElement = 0;
	srvd.Buffer.NumElements = MAX_PARTICLES;
	Renderer::GetDevice()->CreateShaderResourceView(*buffer, &srvd, srv);
}

void GeometryInstancingMaterial::GeometryInstancing(int num)
{
	Renderer::GetDeviceContext()->VSSetShaderResources(5, 1, &scaleSRV);
	Renderer::GetDeviceContext()->VSSetShaderResources(6, 1, &rotationSRV);
	Renderer::GetDeviceContext()->VSSetShaderResources(7, 1, &positionSRV);
	Renderer::GetDeviceContext()->DrawInstanced(4, num, 0, 0);
}