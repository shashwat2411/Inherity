#include "customScenes.h"
#include <thread>

ID3D11ShaderResourceView* LOAD_SCENE::backTexture = nullptr;
ID3D11ShaderResourceView* LOAD_SCENE::texture = nullptr;
bool LOAD_SCENE::loadOver = false;
bool LOAD_SCENE::logo = true;

bool switchero = false;
bool cameraColor = false;

IMAGE* background;
IMAGE* loader;

void LOAD_SCENE::BeforeInit()
{
	logo = true;

	end = false;

	switchero = false;
	cameraColor = false;

	name = "Load";

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\LoadScreen.png",
		NULL,
		NULL,
		&backTexture,
		NULL);
	assert(backTexture);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\load.png",
		NULL,
		NULL,
		&texture,
		NULL);
	assert(texture);
}

void LOAD_SCENE::Init()
{
	//GAMEOBJECT
	MainCamera = AddGameObject<CAMERA>("MainCamera", CAMERA_LAYER);

	//UI
	background = AddGameObject<IMAGE>("Background", SPRITE_LAYER);
	loader = AddGameObject<IMAGE>("Load", SPRITE_LAYER);

	//親子関係設定
	{

	}

	//設定
	{
		MainCamera->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

		background->GetMaterial()->SetTexture("_Texture", backTexture);
		background->sprite->SetSize(D3DXVECTOR2(16.0f, 9.0f));
		background->transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		background->transform->Scale = D3DXVECTOR3(30.0f, 30.0f, 30.0f);
		background->SetColor(D3DXCOLOR(1.0f, 1.0, 1.0f, 0.0f));

		loader->GetMaterial()->SetTexture("_Texture", texture);
		loader->sprite->SetSize(D3DXVECTOR2(20.0f, 20.0f));
		loader->transform->Position = D3DXVECTOR3(930.0f, 510.0f, 0.0f);
	}

	//音
	{

	}


	std::thread th(&Manager::Load);
	th.detach();

}

void LOAD_SCENE::Update()
{
	loader->transform->Rotation.z += 0.5f;

	if (switchero == false)
	{
		background->SetColor(Vector4::Lerp(background->GetColor(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.05f));
		if (background->GetColor().a >= 0.99f) { switchero = true; }
	}
	else
	{
		background->SetColor(Vector4::Lerp(background->GetColor(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), 0.1f));
		if (background->GetColor().a <= 0.01f) { cameraColor = true; }
	}

	if (cameraColor == true)
	{
		MainCamera->SetColor(Vector4::Lerp(MainCamera->GetColor(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.05f));
	}

	if (loadOver == true)
	{
		logo = false;

		texture->Release();
		backTexture->Release();

		Manager::SetScene<TITLE_SCENE>();

	}
}