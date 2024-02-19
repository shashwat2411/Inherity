#include "customScenes.h"
#include "input.h"
#include "postProcessManager.h"


bool startAnimation = false;

int choice = 0;
float pressTimer = 0.0f;

IMAGE* choices[2];
GAMEOBJECT* PlayerModel;

D3DXVECTOR3 selectedScale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
D3DXCOLOR selectedColor = D3DXCOLOR(1.0f, 0.0f, 0.506f, 1.0f);

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

	name = "title";

	//ïœêî
	BILLBOARD* pear;
	IMAGE* tomato;
	IMAGE* revenge;

	//GAMEOBJECT
	skyDome = AddGameObject<SKYDOME>("SkyDome");
	player = AddGameObject<PLAYER>("Player");
	PlayerModel = AddGameObject<PLAYERMODEL>("Player Model");

	pear = AddGameObject<BILLBOARD>("Pear", BILLBOARD_LAYER);

	//UI
	tomato = AddGameObject<IMAGE>("Tomato's", SPRITE_LAYER);
	revenge = AddGameObject<IMAGE>("Revenge", SPRITE_LAYER);
	choices[0] = AddGameObject<IMAGE>("Play", SPRITE_LAYER);
	choices[1] = AddGameObject<IMAGE>("Quit", SPRITE_LAYER);

	//GAMEOBJECT* logo = AddGameObject<IMAGE>("Logo", SPRITE_LAYER);

	//ê⁄ë±èàóù
	{
		PlayerModel->SetParent(player);
		player->RemoveComponent<PlayerMovement>();
		player->RemoveComponent<Rigidbody>();
		player->RemoveComponent<PlayerControl>();
		player->RemoveComponent<PlayerHealth>();



	}

	//ê›íË
	{
		skyDome->GetComponent<MeshFilter>()->SetModel(ModelReader::SKYDOME_M);
		skyDome->AddMaterial<UnlitMaterial>()->SetTexture("_Texture", TextureReader::BOX_T);

		pear->billboard->SetSize(D3DXVECTOR2(0.838f, 1.0f));
		pear->GetMaterial()->SetTexture("_Texture", TextureReader::PEAR_DEATH_T);
		pear->billboard->flip = false;

		//UI
		//logo->GetMaterial()->SetTexture("_Texture", TextureReader::TITLE_BG_T);
		//logo->GetComponent<SpriteRenderer>()->SetSize(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
		//logo->transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		
		choices[0]->GetMaterial()->SetTexture("_Texture", TextureReader::PLAY_TEXT_T);
		choices[1]->GetMaterial()->SetTexture("_Texture", TextureReader::QUIT_TEXT_T);

	}

	//âπ
	{
		//SoundReader::GetReadSound(SoundReader::TITLE)->Play(true, 0.2f);
		MainCamera->AddComponent<AudioSource>();
	}

	startAnimation = false;

	PlayerModel->GetComponent<MeshFilter>()->SetDefaultAnimation("Anger");

	RemoveGameObject("Damager");
	RemoveGameObject("Gun Left", LATEOBJECT_LAYER);
	RemoveGameObject("Gun Right", LATEOBJECT_LAYER);
	RemoveGameObject("Spawn Point Left", LATEOBJECT_LAYER);
	RemoveGameObject("Spawn Point Right", LATEOBJECT_LAYER);
	RemoveGameObject("Damager", COLLIDER_LAYER);
	RemoveGameObject("Player HP", SPRITE_LAYER);
	RemoveGameObject("Player Icon", SPRITE_LAYER);
	RemoveGameObject("Sliced", SPRITE_LAYER);

	GAMEOBJECT* enemies[3];
	for (int i = 0; i < 3; i++)
	{
		std::string name = "Enemy(Clone)_" + std::to_string(i + 1);
		enemies[i] = AddGameObject<ENEMY>(name);
		enemies[i]->GetChildren()[0]->GetComponent<MeshFilter>()->SetDefaultAnimation("Enemy_Pose_1");
		enemies[i]->transform->Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		enemies[i]->RemoveComponent<EnemyHealth>();
		enemies[i]->RemoveComponent<ArtificialIntelligence>();
		enemies[i]->RemoveComponent<EnemyScript>();
		enemies[i]->RemoveComponent<SphereCollider>();
		enemies[i]->RemoveComponent<Rigidbody>();

		RemoveGameObject("Knife");
		RemoveGameObject(name, COLLIDER_LAYER);
		RemoveGameObject("Knife", COLLIDER_LAYER);
		RemoveGameObject("Point 1", GIZMO_LAYER);
		RemoveGameObject("Point 2", GIZMO_LAYER);
		RemoveGameObject("Point 3", GIZMO_LAYER);
		RemoveGameObject("Point 4", GIZMO_LAYER);
		RemoveGameObject("Enemy HP", BILLBOARD_LAYER);
	}

	enemies[0]->transform->Position = D3DXVECTOR3(-4.6f, 3.0f, -4.6f);
	enemies[1]->transform->Position = D3DXVECTOR3(-2.15f, 4.3f, -3.55f);
	enemies[2]->transform->Position = D3DXVECTOR3(0.003f, 3.0f, -5.375f);

	enemies[0]->GetChildren()[0]->transform->Rotation = D3DXVECTOR3(-4.0f, 181.0f, 0.0f);
	enemies[1]->GetChildren()[0]->transform->Rotation = D3DXVECTOR3(-4.0f, 228.0f, 0.0f);
	enemies[2]->GetChildren()[0]->transform->Rotation = D3DXVECTOR3(-4.0f, 286.5f, 0.0f);

	enemies[0]->GetChildren()[0]->SetColor(D3DXCOLOR(0.661f, 0.015f, 0.313f, 0.0f));
	enemies[1]->GetChildren()[0]->SetColor(D3DXCOLOR(0.415f, 0.0f, 0.0f, 0.0f));
	enemies[2]->GetChildren()[0]->SetColor(D3DXCOLOR(0.346f, 0.16f, 0.29f, 0.0f));

	MainCamera->camera->SetLightDirection(D3DXVECTOR3(-1.41f, 3.19f, 1.93f));
}

void TITLE_SCENE::LateInit()
{
	//PostProcessManager::RemovePoster<PostProcessMaterial>();
	PostProcessManager::AddPoster<DrunkMaterial>();

}

void TITLE_SCENE::Update()
{

	if (Input::GetButtonTrigger(LEFT_KEYMAP)) { if (choice > 0) { choice--; } else { choice = 1; } SoundReader::GetReadSound(SoundReader::OPTION_CHANGE)->Play(false, 0.4f);}
	else if (Input::GetButtonTrigger(RIGHT_KEYMAP)) { if (choice < 1) { choice++; } else { choice = 0; } SoundReader::GetReadSound(SoundReader::OPTION_CHANGE)->Play(false, 0.4f); }
	
	if (end == false)
	{
		for (int i = 0; i < 2; i++)
		{
			choices[i]->transform->Scale = D3DXVECTOR3(0.25f, 0.25f, 0.25f);
			choices[i]->SetColor(D3DXCOLOR(0.75f, 0.75f, 0.75f, 0.75f));
		}

		choices[choice]->transform->Scale = selectedScale;
		choices[choice]->SetColor(selectedColor);
	}

	if (Input::GetButtonTrigger(CHANGE_KEYMAP)) 
	{ 
		end = true; 
		SoundReader::GetReadSound(SoundReader::OPTION_SELECT)->Play(false, 0.2f);
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
				if (choice == 0) { Manager::SetScene<GAME_SCENE>(); }
				else { exit(999); }
			}
		}
	}
}

bool Press()
{
	if (Time::WaitForSeconds(0.1f, &pressTimer) == false) 
	{
		choices[choice]->transform->Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
		Input::SetControls(false);

		return false; 
	}
	else if (Time::WaitForSeconds(1.2f, &pressTimer) == false)
	{
		choices[choice]->transform->Scale = selectedScale;
		return false;
	}

	return true;
}