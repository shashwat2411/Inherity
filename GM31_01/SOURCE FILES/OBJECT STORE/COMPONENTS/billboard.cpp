#include "component.h"
#include "manager.h"
#include "material.h"


void Billboard::Start()
{
	animate = false;
	atc = false;

	elementsX = 1;
	elementsY = 1;

	Offset.x = 0.0f;
	Offset.y = 0.0f;

	Count = 0;

	Size = D3DXVECTOR2(10.0f, 10.0f);
	TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	collisionObject = nullptr;

	//----------------------------------------------------------------
	float x = Count % elementsX * (1.0f / (float)elementsX);
	float y = Count / elementsY * (1.0f / (float)elementsX);
	D3DXVECTOR2 offset = D3DXVECTOR2((1.0f / (float)elementsX), (1.0f / (float)elementsY));

	gameObject->AddMaterial<SpriteMaterial>();
	gameObject->SetBillboard(true);

	{
		VERTEX_3D vertex[4];
		vertex[0].Position = D3DXVECTOR3(-Size.x + Offset.x, Size.y + Offset.y, 0.0f);
		vertex[0].Normal = Normal;
		vertex[0].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
		vertex[0].TexCoord = D3DXVECTOR2(x, y);

		vertex[1].Position = D3DXVECTOR3(Size.x + Offset.x, Size.y + Offset.y, 0.0f);
		vertex[1].Normal = Normal;
		vertex[1].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
		vertex[1].TexCoord = D3DXVECTOR2(x + offset.x, y);

		vertex[2].Position = D3DXVECTOR3(-Size.x + Offset.x, -Size.y + Offset.y, 0.0f);
		vertex[2].Normal = Normal;
		vertex[2].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
		vertex[2].TexCoord = D3DXVECTOR2(x, y + offset.y);

		vertex[3].Position = D3DXVECTOR3(Size.x + Offset.x, -Size.y + Offset.y, 0.0f);
		vertex[3].Normal = Normal;
		vertex[3].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
		vertex[3].TexCoord = D3DXVECTOR2(x + offset.x, y + offset.y);


		//頂点バッファー生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		if (animate == false)
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &VertexBuffer);
	}
}

void Billboard::End()
{
	if (VertexBuffer != nullptr) { VertexBuffer->Release(); }
}

void Billboard::Update()
{

}

void Billboard::Draw()
{
	Normal.x = gameObject->GetWorldMatrix()._21;
	Normal.y = gameObject->GetWorldMatrix()._22;
	Normal.z = gameObject->GetWorldMatrix()._23;

	float x = 0.0f;
	float y = 0.0f;

	D3DXVECTOR2 offset = D3DXVECTOR2(1.0f, 1.0f);
	if (animate == true)
	{
		offset = D3DXVECTOR2((1.0f / (float)elementsX), (1.0f / (float)elementsY));
		x = (Count % elementsX) * offset.x;
		y = (Count / elementsX) * offset.y;
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	vertex[0].Position = D3DXVECTOR3(-Size.x + Offset.x, Size.y + Offset.y, 0.0f);
	vertex[0].Normal = Normal;
	vertex[0].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
	vertex[0].TexCoord = D3DXVECTOR2(x, y);

	vertex[1].Position = D3DXVECTOR3(Size.x + Offset.x, Size.y + Offset.y, 0.0f);
	vertex[1].Normal = Normal;
	vertex[1].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
	vertex[1].TexCoord = D3DXVECTOR2(x + offset.x, y);

	vertex[2].Position = D3DXVECTOR3(-Size.x + Offset.x, -Size.y + Offset.y, 0.0f);
	vertex[2].Normal = Normal;
	vertex[2].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
	vertex[2].TexCoord = D3DXVECTOR2(x, y + offset.y);

	vertex[3].Position = D3DXVECTOR3(Size.x + Offset.x, -Size.y + Offset.y, 0.0f);
	vertex[3].Normal = Normal;
	vertex[3].Diffuse = (D3DXVECTOR4)gameObject->GetColor();
	vertex[3].TexCoord = D3DXVECTOR2(x + offset.x, y + offset.y);

	if (flip == true)
	{
		vertex[0].TexCoord = D3DXVECTOR2(x + offset.x, y);
		vertex[1].TexCoord = D3DXVECTOR2(x, y);
		vertex[2].TexCoord = D3DXVECTOR2(x + offset.x, y + offset.y);
		vertex[3].TexCoord = D3DXVECTOR2(x, y + offset.y);
	}

	Renderer::GetDeviceContext()->Unmap(VertexBuffer, 0);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(gameObject->GetVertexLayout());

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(gameObject->GetVertexShader(), NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(gameObject->GetPixelShader(), NULL, 0);

	//Animation


	//頂点バッファの設定
	UINT stride = sizeof(VERTEX_3D);
	UINT Offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &Offset);

	////マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (atc == true) { Renderer::SetATCEnable(true); }
	//ポリゴン設定
	Renderer::GetDeviceContext()->Draw(4, 0);

	if (atc == true) { Renderer::SetATCEnable(false); }
}

void Billboard::EngineDisplay()
{
	if (ImGui::TreeNode("Billboard"))
	{
		ImGui::Text("Texture : ");
		ImGui::SameLine();

		ImGui::PushID(0);
		if (ImGui::Combo("", gameObject->GetMaterial()->GetIndex(), TextureReader::GetTextureNames(), TextureReader::READ_TEXTURE_MAX))
		{
			gameObject->GetMaterial()->SetTexture("_Texture", ((TextureReader::READ_TEXTURE)*gameObject->GetMaterial()->GetIndex()));
		}
		ImGui::PopID();

		DebugManager::BoolDisplay(&animate, -200.0f, "Animate", 1);
		ImGui::SameLine();
		DebugManager::BoolDisplay(&flip, -146.0f, "Flip", 2);
		
		ImGui::Text("\n");

		ImGui::DragFloat2("Size", Size, 0.1F);
		ImGui::DragFloat2("TexCoord", TexCoord, 0.1F);
		ImGui::DragFloat2("Offset", Offset, 0.1F);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}
