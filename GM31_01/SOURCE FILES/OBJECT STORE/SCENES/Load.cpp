#include "customScenes.h"
#include <thread>

ID3D11ShaderResourceView* LOAD_SCENE::texture = nullptr;
ID3D11ShaderResourceView* LOAD_SCENE::backTexture = nullptr;
bool LOAD_SCENE::loadOver = false;
bool LOAD_SCENE::logo = true;

IMAGE* result;

void LOAD_SCENE::BeforeInit()
{
	logo = true;

	end = false;

	name = "Load";

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\load.png",
		NULL,
		NULL,
		&texture,
		NULL);
	assert(texture);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\fade.png",
		NULL,
		NULL,
		&backTexture,
		NULL);
	assert(backTexture);
}

void LOAD_SCENE::Init()
{
	//GAMEOBJECT

	//UI
	IMAGE* background = AddGameObject<IMAGE>("Background", SPRITE_LAYER);
	result = AddGameObject<IMAGE>("Load", SPRITE_LAYER);

	//親子関係設定
	{

	}

	//設定
	{
		background->GetMaterial()->SetTexture("_Texture", backTexture);
		background->sprite->SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
		background->transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		background->SetColor(D3DXCOLOR(0.0f, 0.0, 0.0f, 1.0f));

		result->GetMaterial()->SetTexture("_Texture", texture);
		result->sprite->SetSize(D3DXVECTOR2(60.0f, 60.0f));
		result->transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	}

	//音
	{

	}


	std::thread th(&Manager::Load);
	th.detach();

}

void LOAD_SCENE::Update()
{
	result->transform->Rotation.z += 1.0f;

	if (loadOver == true)
	{
		logo = false;

		texture->Release();
		backTexture->Release();

		Manager::SetScene<TITLE_SCENE>();
	}
}