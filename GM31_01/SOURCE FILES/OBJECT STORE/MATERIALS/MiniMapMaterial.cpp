#include "material.h"
#include "manager.h"

GAMEOBJECT* player;
GAMEOBJECT* map;

void MiniMapMaterial::Start()
{
	SetFloat("_Radius", 0.45f);
	SetFloat("_Outline", 0.04f);
	SetFloat("_Dot_Radius", 0.021f);

	SetTexture("_Texture", TextureReader::MINIMAP_T);

	Renderer::CreateVertexShader(gameObject->GetVertexShaderPointer(), gameObject->GetVertexLayoutPointer(), "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(gameObject->GetPixelShaderPointer(), "shader\\miniMapPS.cso");

	player = Manager::GetScene()->GetPlayer();
	map = Manager::GetScene()->Find("Map");
}

void MiniMapMaterial::Update()
{

}

void MiniMapMaterial::Draw()
{
	if (textures["_Texture"] != nullptr) { Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &textures["_Texture"]); }

	D3DXVECTOR3 coordinates = player->transform->Position / 200.0f;
	//D3DXVec3TransformCoord(&coordinates, &player->transform->Position, &map->GetWorldMatrix());
	//D3DXVec3Normalize(&coordinates, &coordinates);

	PARAMETER param;
	ZeroMemory(&param, sizeof(param));
	param.color = D3DXCOLOR(coordinates.x, 0.0f, -coordinates.z, 0.0f);
	param.color2 = D3DXCOLOR(0.0f, floats["_Radius"], floats["_Outline"], floats["_Dot_Radius"]);
	param.color3 = Manager::GetScene()->GetCamera()->GetColor();

	Renderer::SetParameter(param);
}