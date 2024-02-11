#include "customScenes.h"
#include "input.h"

bool startAnimation = false;

int choice = 0;
float pressTimer = 0.0f;

IMAGE* choices[3];
GAMEOBJECT* PlayerModel;

D3DXVECTOR3 selectedScale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
D3DXCOLOR selectedColor = D3DXCOLOR(1.0f, 1.0f, 1.0, 1.0f);

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
	choices[1] = AddGameObject<IMAGE>("Tutorial", SPRITE_LAYER);
	choices[2] = AddGameObject<IMAGE>("Quit", SPRITE_LAYER);

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
		choices[1]->GetMaterial()->SetTexture("_Texture", TextureReader::TUTORIAL_TEXT_T);
		choices[2]->GetMaterial()->SetTexture("_Texture", TextureReader::QUIT_TEXT_T);

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


	MainCamera->camera->SetLightDirection(D3DXVECTOR3(-0.9f, 1.71f, 2.63f));
}

void TITLE_SCENE::LateInit()
{
	//GameObjects[FADE_LAYER].push_back(Manager::GetDontDestroyOnLoadScene()->Find("Fade"));
}

void TITLE_SCENE::Update()
{

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