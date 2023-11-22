#include "component.h"
#include "manager.h"
#include "material.h"

void SpriteRenderer::Start()
{
	animate = false;
	ignorePause = true;

	count = 0;

	elementsX = 1;
	elementsY = 1;

	barOffsetRight = 0.0f;
	barOffsetLeft = 0.0f;

	TexCoord = D3DXVECTOR2(1.0f, 1.0f);
	Size = D3DXVECTOR2(100.0f, 100.0f);
	Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//----------------------------------------------------------------
	gameObject->AddMaterial<SpriteMaterial>();
	gameObject->SetDepth(false);

	{
		VERTEX_3D vertex[4];
		//頂点バッファー生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &VertexBuffer);
	}

}
void SpriteRenderer::End()
{
	VertexBuffer->Release();
}
void SpriteRenderer::Update()
{
	if (count >= elementsX * elementsY)
	{
		count = 0;
	}
}
void SpriteRenderer::Draw()
{
	float x = 0.0f;
	float y = 0.0f;

	D3DXVECTOR2 offset = D3DXVECTOR2(1.0f, 1.0f);

	if (animate == true)
	{
		offset = D3DXVECTOR2((1.0f / (float)elementsX), (1.0f / (float)elementsY));
		x = (count % elementsX) * offset.x;
		y = (count / elementsX) * offset.y;
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D vertex[4];
	vertex[0].Position = D3DXVECTOR3(-Size.x * gameObject->transform->Scale.x + barOffsetLeft, -Size.y * gameObject->transform->Scale.y, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
	vertex[0].TexCoord = D3DXVECTOR2(x, y);

	vertex[1].Position = D3DXVECTOR3(Size.x * gameObject->transform->Scale.x + barOffsetRight, -Size.y * gameObject->transform->Scale.y, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
	vertex[1].TexCoord = D3DXVECTOR2(x + offset.x, y);

	vertex[2].Position = D3DXVECTOR3(-Size.x * gameObject->transform->Scale.x + barOffsetLeft, Size.y * gameObject->transform->Scale.y, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
	vertex[2].TexCoord = D3DXVECTOR2(x, y + offset.y);

	vertex[3].Position = D3DXVECTOR3(Size.x * gameObject->transform->Scale.x + barOffsetRight, Size.y * gameObject->transform->Scale.y, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
	vertex[3].TexCoord = D3DXVECTOR2(x + offset.x, y + offset.y);

	Renderer::GetDeviceContext()->Unmap(VertexBuffer, 0);

	float rad = D3DXToRadian(gameObject->transform->Rotation.z);

	for (int i = 0; i < 4; i++)
	{
		float X = ((vertex[i].Position.x * cosf(rad)) - (vertex[i].Position.y * sinf(rad)));
		float Y = ((vertex[i].Position.x * sinf(rad)) + (vertex[i].Position.y * cosf(rad)));
		vertex[i].Position.x = gameObject->transform->Position.x + X;
		vertex[i].Position.y = gameObject->transform->Position.y + Y;
		vertex[i].Position.z = 0.0f;
	}

	
	Renderer::GetDeviceContext()->IASetInputLayout(gameObject->GetVertexLayout());

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(gameObject->GetVertexShader(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(gameObject->GetPixelShader(), NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//頂点バッファの設定
	UINT stride = sizeof(VERTEX_3D);
	UINT Offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &Offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = Color;
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン設定
	Renderer::GetDeviceContext()->Draw(4, 0);

}