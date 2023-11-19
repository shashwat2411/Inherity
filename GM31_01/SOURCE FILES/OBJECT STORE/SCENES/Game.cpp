#include "customScenes.h"

NUMBER* Score;
GAMEOBJECT* empty2;
GAMEOBJECT* empty1;
GAMEOBJECT* PlayerModel;
PLANE* Field;
PLANE* Water;
IMAGE* Buffer;
CUBE* cube;
GAMEOBJECT* gameManager;
AudioSource* audio;

void GAME_SCENE::Init()
{
	//変数
	GAMEOBJECT* Cylinder;
	ENEMY* enemy;
	GAMEOBJECT* rock[20];
	BILLBOARD* tree[300];
	GAMEOBJECT* torus;

	//GAMEOBJECT
	skyDome = AddGameObject<SKYDOME>(GAMEOBJECT_LAYER);
	gameManager = AddGameObject<EMPTYOBJECT>();
	player = AddGameObject<PLAYER>();
	PlayerModel = AddGameObject<PLAYERMODEL>();
	enemy = AddGameObject<ENEMY>();
	Field = AddGameObject<PLANE>();
	Water = AddGameObject<PLANE>();
	cube = AddGameObject<CUBE>();
	torus = AddGameObject<EMPTYOBJECT>();

	srand(0);	//Seed Value for the random numbers
	//Field Objects
	/*
	for (int i = 0; i < 20; i++)
	{
		D3DXVECTOR3 position;

		position.x = (float)(rand() % 100 - 50);
		position.y = 1.0f;
		position.z = (float)(rand() % 100 - 50);

		position.y = Field->GetComponent<MeshField>()->GetHeight(position);

		rock[i] = AddGameObject<ROCK>();
		rock[i]->transform->Position = position;
	}

	for (int i = 0; i < 300; i++)
	{
		D3DXVECTOR3 position;

		position.x = (float)(rand() % 100 - 50);
		position.y = 1.0f;
		position.z = (float)(rand() % 100 - 50) + (float)(rand() % 3 - 1) / 100.0f;

		position.y = Field->GetComponent<MeshField>()->GetHeight(position);

		tree[i] = AddGameObject<TREE>();
		tree[i]->transform->Position = position;

		float offset;
		offset = (float)(rand() % 30 + 10);

		tree[i]->billboard->SetSize(D3DXVECTOR2(offset, offset));
		tree[i]->billboard->SetOffset(Billboard::Y);
	}
	*/

	//UI
	Buffer = AddGameObject<IMAGE>(SPRITE_LAYER);
	Score = AddGameObject<NUMBER>(SPRITE_LAYER);


	//接続処理
	{
		PlayerModel->SetParent(player);
		MainCamera->GetComponent<Camera>()->Target = player;
	}

	//設定
	{
		gameManager->AddComponent<GameManager>();

		Field->GetMaterial()->SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::GROUND_T));
		Field->meshField->TexCoord = D3DXVECTOR2(10.0f, 10.0f);
		Field->meshField->Size = D3DXVECTOR2(5.0f, 5.0f);
		Field->transform->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Field->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//Field->transform->Rotation = D3DXVECTOR3(0.84f, 0.0f, 0.0f);
		Field->meshField->RecreateField();

		Water->AddMaterial<WaterMaterial>();
		Water->GetMaterial()->SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::WATER_T));
		Water->meshField->TexCoord = D3DXVECTOR2(10.0f, 10.0f);
		Water->meshField->Size = D3DXVECTOR2(5.0f, 5.0f);
		Water->transform->Scale = D3DXVECTOR3(1.0f, 1.0f, 0.1f);
		Water->transform->Position = D3DXVECTOR3(0.0f, 0.29f, 0.0f);
		Water->transform->Rotation = D3DXVECTOR3(1.44f, 0.0f, 0.0f);
		Water->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
		Water->SetDepthShadow(false);
		Water->meshField->RecreateField();

		cube->transform->Position.y = 2.0f;
		cube->transform->Scale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
		cube->AddMaterial<LitTextureMaterial>();
		//cube->GetMaterial()->SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::RING_T));
		cube->SetActive(false);

		Buffer->transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 7, SCREEN_HEIGHT / 2 - 150.0f, 0.0f);
		Buffer->transform->Scale = D3DXVECTOR3(1.13f, 1.13f, 1.13f);

		Score->transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, 30.0f, 0.0f);
		Score->transform->Scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		Score->SetDigits(3);

		torus->AddComponent<MeshFilter>()->SetModel(ModelReader::GetReadModel(ModelReader::TORUS_M));
		torus->transform->Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	//音
	{
		player->AddComponent<AudioListener>();

		//audio = MainCamera->AddComponent<AudioSource>();
		//audio->clip = SoundReader::GetReadSound(SoundReader::GAME);
		//audio->Play(true, 0.2f);

		audio = enemy->AddComponent<AudioSource>();
		audio->clip = SoundReader::GetReadSound(SoundReader::GAME);
		audio->SetThreeDimension(true);
		audio->Play(true, 0.2f);
	}
}

float r = 0.0f;
float g = 0.0f;
float b = 0.0f;
float speed1 = 0.01f;
float speed2 = 0.02f;
float speed3 = 0.03f;

void GAME_SCENE::Update()
{
	if (r > 1.0f || r < 0.0f) { speed1 *= -1.0f; }
	r += speed1 * Time::fixedTimeScale;

	if (g > 1.0f || g < 0.0f) { speed2 *= -1.0f; }
	g += speed2 * Time::fixedTimeScale;

	if (b > 1.0f || b < 0.0f) { speed3 *= -1.0f; }
	b += speed3 * Time::fixedTimeScale;

	//r = sinf(r);
	//g = sinf(g);
	//b = sinf(b);

	if (Input::GetKeyTrigger('Z')) { GetCamera()->camera->CameraShake(1.0f); }
	if (Input::GetKeyTrigger('Z')) { Score->Increment(10); }

	if (Input::GetKeyTrigger(VK_RETURN)) { end = true; }

	//if (Input::GetKeyPress('Q')) { Water->transform->Position.y += 0.01f; }
	//if (Input::GetKeyPress('E')) { Water->transform->Position.y -= 0.01f; }
	//
	//if (Input::GetKeyPress('R')) { audio->volume += 0.01f; }
	//if (Input::GetKeyPress('T')) { audio->volume -= 0.01f; }

	cube->SetColor(D3DXCOLOR(r, g, b, 1.0f));


	ID3D11ShaderResourceView* depthShadowTexture;
	depthShadowTexture = Renderer::GetDepthShadowTexture();
	Buffer->GetMaterial()->SetTexture("_Texture", depthShadowTexture);

	if (end == true && Fade->GetFadeIn() == false) { if (Fade->FadeOut() == false) { Manager::SetScene<RESULT_SCENE>(); } }

#ifdef DEBUG	// デバッグ情報を表示する
	char* str = GetDebugStr();
	//sprintf(&str[strlen(str)], " |||  Number : %d, Player_L_Joint1 x : %.2f, y : %.2f, z : %.2f", GameObjects[SHADOW_LAYER].Size(), PlayerModel->L_joint1->transform->Position.x, PlayerModel->L_joint1->transform->Position.y, PlayerModel->L_joint1->transform->Position.z);
	//sprintf(&str[strlen(str)], " | Player_L_arm1  x : %.2f, y : %.2f, z : %.2f", PlayerModel->L_arm1->transform->Position.x, PlayerModel->L_arm1->transform->Position.y, PlayerModel->L_arm1->transform->Position.z);
	//sprintf(&str[strlen(str)], " | Water Y : %.2f", Water->transform->Position.y);
	//sprintf(&str[strlen(str)], " | Water Rot X : %.2f", Water->transform->Rotation.x);
	//sprintf(&str[strlen(str)], " | Buffer Scale : %.2f", Buffer->transform->Scale.x);
	//sprintf(&str[strlen(str)], " | Volume : %.2f", audio->volume);
	sprintf(&str[strlen(str)], " | Volume Percentage : %.2f", audio->volumePercentage);
#endif
}