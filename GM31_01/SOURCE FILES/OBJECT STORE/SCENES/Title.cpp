#include "customScenes.h"
#include "input.h"

int choice = 0;
float pressTimer = 0.0f;

IMAGE* choices[3];
GAMEOBJECT* playerModel;

D3DXVECTOR3 selectedScale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);

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
	}

	//‰¹
	{
		//SoundReader::GetReadSound(SoundReader::GAME)->Play(true, 0.2f);
	}
}

void TITLE_SCENE::Update()
{
	playerModel->GetComponent<MeshFilter>()->SetAnimationBlend("Idle", true);

	if (Input::GetButtonTrigger(FORWARD_KEYMAP)) { if (choice > 0) { choice--; } else { choice = 2; } SoundReader::GetReadSound(SoundReader::GUARD)->Play(false, 0.2f);}
	else if (Input::GetButtonTrigger(BACK_KEYMAP)) { if (choice < 2) { choice++; } else { choice = 0; } SoundReader::GetReadSound(SoundReader::GUARD)->Play(false, 0.2f); }
	
	if (end == false)
	{
		for (int i = 0; i < 3; i++)
		{
			choices[i]->transform->Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
		}

		choices[choice]->transform->Scale = selectedScale;
	}

	if (Input::GetButtonTrigger(CHANGE_KEYMAP)) 
	{ 
		end = true; 
		SoundReader::GetReadSound(SoundReader::GUARD)->Play(false, 0.2f);
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
	if (Time::WaitForSeconds(0.07f, &pressTimer) == false) 
	{
		choices[choice]->transform->Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
		Input::SetControls(false);

		return false; 
	}

	choices[choice]->transform->Scale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
	return true;
}