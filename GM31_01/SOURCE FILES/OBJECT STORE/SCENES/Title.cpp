#include "customScenes.h"
#include "input.h"

bool startAnimation = false;

int choice = 0;
float pressTimer = 0.0f;

IMAGE* choices[3];
GAMEOBJECT* playerModel;

D3DXVECTOR3 selectedScale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
D3DXCOLOR selectedColor = D3DXCOLOR(1.0f, 1.0f, 1.0, 1.0f);
PARTICLESYSTEM* ps;

//D3DXVECTOR3 psPosition[3] = {
//	D3DXVECTOR3( 0.3f,   -9.0f, -101.1f),
//	D3DXVECTOR3(1.25f,  -10.5f, -101.1f),
//	D3DXVECTOR3( 0.3f, -11.65f, -101.1f)
//};

D3DXVECTOR3 psPosition[3] = {
	D3DXVECTOR3(8.7f, -12.45f, -101.1f),
	D3DXVECTOR3(8.7f, -13.95f, -101.1f),
	D3DXVECTOR3(8.7f, -15.65f, -101.1f)
};

bool Press();

void TITLE_SCENE::Init()
{
	pressTimer = 0.0f;

	PLANE* Field;
	PLANE* Water;

	name = "title";

	//GAMEOBJECT
	skyDome = AddGameObject<SKYDOME>("SkyDome");
	player = AddGameObject<PLAYER>("Player");
	playerModel = AddGameObject<PLAYERMODEL>("Player Model");
	Field = AddGameObject<PLANE>("Field");
	Water = AddGameObject<PLANE>("Water");
	ps = AddGameObject<PARTICLESYSTEM>("Glass Shards", BILLBOARD_LAYER);

	//UI
	choices[0] = AddGameObject<IMAGE>("Play", SPRITE_LAYER);
	choices[1] = AddGameObject<IMAGE>("Tutorial", SPRITE_LAYER);
	choices[2] = AddGameObject<IMAGE>("Quit", SPRITE_LAYER);

	//GAMEOBJECT* logo = AddGameObject<IMAGE>("Logo", SPRITE_LAYER);

	//Ú‘±ˆ—
	{
		playerModel->SetParent(player);

		player->RemoveComponent<PlayerMovement>();
	}

	//Ý’è
	{
		skyDome->GetComponent<MeshFilter>()->SetModel(ModelReader::TITLE_SKYDOME_M);
		skyDome->AddMaterial<MetallicMaterial>();

		playerModel->GetMaterial()->SetTexture("_Normal_Map", TextureReader::FIELD_NM_T);

		Field->GetMaterial()->SetTexture("_Texture", TextureReader::GROUND_T);
		Field->meshField->TexCoord = D3DXVECTOR2(10.0f, 10.0f);
		Field->meshField->Size = D3DXVECTOR2(5.0f, 5.0f);
		Field->transform->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Field->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//Field->transform->Rotation = D3DXVECTOR3(0.84f, 0.0f, 0.0f);
		Field->meshField->RecreateField();

		playerModel->SetReflection(true);
		Field->SetReflection(true);
		Water->SetReflection(true);

		Water->AddMaterial<WaterMaterial>();
		Water->GetMaterial()->SetTexture("_Texture", TextureReader::WATER_T);
		Water->meshField->TexCoord = D3DXVECTOR2(1.0f, 1.0f);
		Water->meshField->Size = D3DXVECTOR2(5.0f, 5.0f);
		Water->transform->Scale = D3DXVECTOR3(1.0f, 1.0f, 0.1f);
		Water->transform->Position = D3DXVECTOR3(0.0f, 0.29f, 0.0f);
		Water->transform->Rotation = D3DXVECTOR3(1.44f, 0.0f, 0.0f);
		Water->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
		Water->SetDepthShadow(false);
		Water->meshField->RecreateField();

		//UI
		//logo->GetMaterial()->SetTexture("_Texture", TextureReader::TITLE_BG_T);
		//logo->GetComponent<SpriteRenderer>()->SetSize(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
		//logo->transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		
		choices[0]->GetMaterial()->SetTexture("_Texture", TextureReader::PLAY_TEXT_T);
		choices[1]->GetMaterial()->SetTexture("_Texture", TextureReader::TUTORIAL_TEXT_T);
		choices[2]->GetMaterial()->SetTexture("_Texture", TextureReader::QUIT_TEXT_T);

		ps->particleSystem->burst = false;
		ps->particleSystem->gravity = false;
		ps->particleSystem->SetTexture(TextureReader::GLASS_SHARD_T);
		ps->transform->Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
		ps->particleSystem->SetParticleCount(40);

		ps->particleSystem->randomVelocity = D3DXBOOL3(true, true, false);
		ps->particleSystem->randomPosition = D3DXBOOL3(true, true, false);
		ps->particleSystem->positionOffset = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
		ps->particleSystem->velocityOffset = D3DXVECTOR3(1.0f, 1.0f, 0.0f);

		ps->particleSystem->Reinitialize();
	}

	//‰¹
	{
		//SoundReader::GetReadSound(SoundReader::TITLE)->Play(true, 0.2f);
	}

	startAnimation = false;
}

void TITLE_SCENE::Update()
{
	if (Input::GetKeyTrigger('V') || startAnimation == false) { GetPlayer()->GetComponent<Animator>()->PlayAnimation(0, Animation::LOOP); startAnimation = true; }

	playerModel->GetComponent<MeshFilter>()->SetAnimationBlend("Idle", true);

	if (Input::GetButtonTrigger(FORWARD_KEYMAP)) { if (choice > 0) { choice--; } else { choice = 2; } SoundReader::GetReadSound(SoundReader::OPTION_CHANGE)->Play(false, 0.4f);}
	else if (Input::GetButtonTrigger(BACK_KEYMAP)) { if (choice < 2) { choice++; } else { choice = 0; } SoundReader::GetReadSound(SoundReader::OPTION_CHANGE)->Play(false, 0.4f); }
	
	if (end == false)
	{
		for (int i = 0; i < 3; i++)
		{
			choices[i]->transform->Scale = D3DXVECTOR3(0.25f, 0.25f, 0.25f);
			choices[i]->SetColor(D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.9f));
		}

		choices[choice]->transform->Scale = selectedScale;
		choices[choice]->SetColor(selectedColor);
		ps->transform->Position = psPosition[choice];
	}

	if (Input::GetButtonTrigger(CHANGE_KEYMAP)) 
	{ 
		end = true; 
		SoundReader::GetReadSound(SoundReader::OPTION_SELECT)->Play(false, 0.2f);
		ps->particleSystem->Play();
	}

	if (end == true) 
	{
		float v = MainCamera->GetComponent<AudioSource>()->GetVolume();
		v = Mathf::Lerp(v, v / 4, Time::deltaTime * 3.0f);
		MainCamera->GetComponent<AudioSource>()->SetVolume(v);
	}

	if (end == true && Fade->GetFadeIn() == false)
	{
		if (Press() == true)
		{
			if (Fade->FadeOut() == false)
			{
				Input::SetControls(true);
				if (choice == 0 || choice == 1) { Manager::SetScene<GAME_SCENE>(); }
				else if (choice == 2) { exit(999); }
			}
		}
	}
}

bool Press()
{
	if (Time::WaitForSeconds(1.2f, &pressTimer) == false) 
	{
		//choices[choice]->transform->Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
		Input::SetControls(false);

		return false; 
	}

	//choices[choice]->transform->Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
	return true;
}