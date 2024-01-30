#include "customScenes.h"
#include "animations.h"

IMAGE* shadow;
IMAGE* aimer;
IMAGE* pause;

bool paused = false;
bool pauseReturn = false;

void GAME_SCENE::Init()
{
	name = "game";

	//ïœêî
	GAMEOBJECT* PlayerModel;
	GAMEOBJECT* map;
	ENEMY* enemy;
	PLANE* Field;
	PLANE* Water;
	NUMBER* Score;
	AudioSource* audio;
	EMPTYOBJECT* torus;

	//GAMEOBJECT
	skyDome = AddGameObject<SKYDOME>("SkyDome");
	gameManager = AddGameObject<EMPTYOBJECT>("GameManager");
	player = AddGameObject<PLAYER>("Player");
	PlayerModel = AddGameObject<PLAYERMODEL>("Player Model");
	enemy = AddGameObject<ENEMY>("Enemy");
	torus = AddGameObject<EMPTYOBJECT>("Torus");
	Field = AddGameObject<PLANE>("Field");
	Water = AddGameObject<PLANE>("Water");
	map = AddGameObject<EMPTYOBJECT>("Map");


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
	*/
	/*
	for (int i = 0; i < 50; i++)
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
	Score = AddGameObject<NUMBER>("Score", SPRITE_LAYER);
	shadow = AddGameObject<IMAGE>("Shadow Map", SPRITE_LAYER);
	aimer = AddGameObject<IMAGE>("Aimer", SPRITE_LAYER);
	pause = AddGameObject<IMAGE>("Pause Menu", SPRITE_LAYER);

	//ê⁄ë±èàóù
	{
		PlayerModel->SetParent(player);

		reflectionProjector->transform->Position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);

		MainCamera->AddComponent<RevolutionCamera>();
		MainCamera->camera->SetTarget(player);
	}

	//ê›íË
	{
		gameManager->AddComponent<GameManager>();

		skyDome->GetComponent<MeshFilter>()->SetModel(ModelReader::TITLE_SKYDOME_M);
		skyDome->transform->Scale = D3DXVECTOR3(300.0f, 300.0f, 300.0f);

		//PlayerModel->GetMaterial()->SetTexture("_Normal_Map", TextureReader::FIELD_NM_T);

		Field->GetMaterial()->SetTexture("_Texture", TextureReader::GRASS_T);
		//Field->meshField->SetTiles(40);
		Field->meshField->TexCoord = D3DXVECTOR2(10.0f, 10.0f);
		Field->meshField->Size = D3DXVECTOR2(50.0f, 50.0f);
		Field->transform->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Field->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		Field->meshField->RecreateField();
		Field->SetActive(false);

		PlayerModel->SetReflection(true);
		enemy->SetReflection(true);
		enemy->GetChildren()[0]->SetReflection(true);
		Field->SetReflection(true);
		Water->SetReflection(true);
		map->SetReflection(true);

		torus->AddComponent<MeshFilter>();
		torus->AddMaterial<MetallicMaterial>();

		//Water->AddMaterial<WaterMaterial>();
		Water->GetMaterial()->SetTexture("_Texture", TextureReader::WATER_T);
		Water->meshField->TexCoord = D3DXVECTOR2(10.0f, 10.0f);
		Water->meshField->Size = D3DXVECTOR2(5.0f, 5.0f);
		Water->transform->Scale = D3DXVECTOR3(1.0f, 1.0f, 0.1f);
		Water->transform->Position = D3DXVECTOR3(0.0f, 0.29f, 0.0f);
		Water->transform->Rotation = D3DXVECTOR3(1.44f, 0.0f, 0.0f);
		Water->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
		Water->SetDepthShadow(false);
		Water->meshField->RecreateField();
		Water->SetActive(false);

		map->AddComponent<MeshFilter>()->SetModel(ModelReader::MAP_M);
		map->AddMaterial<FieldDefaultMaterial>();
		map->transform->culling = false;

		//UI
		Score->transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, 30.0f, 0.0f);
		Score->transform->Scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		Score->SetDigits(3);

		shadow->transform->Scale = D3DXVECTOR3(1.13f, 1.13f, 1.13f);

		aimer->AddComponent<ScreenToWorld>();
		aimer->GetMaterial()->SetTexture("_Texture", TextureReader::AIM_T);

		pause->AddComponent<Animator>()->AddAnimation<PauseMenu>();
		pause->GetComponent<Animator>()->AddAnimation<PauseMenuReturn>();
		pause->GetComponent<Animator>()->SetUntimed(true);
		pause->GetComponent<Animator>()->SetCurrentIndex(0);

		player->GetComponent<PlayerMovement>()->aimPoint = aimer->GetComponent<ScreenToWorld>()->GetPoint();
	}

	//âπ
	{
		//player->AddComponent<AudioListener>();

		/*audio = enemy->AddComponent<AudioSource>();
		audio->SetClip(SoundReader::GAME);
		audio->SetThreeDimension(true);
		audio->SetPlayOnAwake(false);*/
		//audio->Play(true, 0.2f);
	}

	paused = false;
	pauseReturn = false;
}

void GAME_SCENE::Update()
{
	if (end == true && Fade->GetFadeIn() == false) { if (Fade->FadeOut() == false) { Manager::SetScene<RESULT_SCENE>(); } }
	if (end == false)
	{
		aimer->transform->Position = D3DXVECTOR3(ImGui::GetMousePos().x, ImGui::GetMousePos().y, 0.0f);
		shadow->GetMaterial()->SetTexture("_Texture", *Renderer::GetDepthShadowTexture());
	}

	Animator* pauser = pause->GetComponent<Animator>();
	if (Input::GetButtonTrigger(PAUSE_KEYMAP) && pauser->GetAnimationState(0) != Animation::PLAYBACK && pauser->GetAnimationState(1) != Animation::PLAYBACK)
	{
		if (paused == false)
		{
			Time::timeScale = 0.0f;
			pauser->PlayAnimation(0, Animation::PLAYBACK);
			paused = true;
		}
		else
		{
			pauser->PlayAnimation(1, Animation::PLAYBACK);
			pauseReturn = true;
		}
	}

	if (pauseReturn == true)
	{
		if (pauser->GetAnimationState(1) == Animation::END)
		{
			pauseReturn = false;
			paused = false;
			Time::timeScale = 1.0f;
		}
	}

}