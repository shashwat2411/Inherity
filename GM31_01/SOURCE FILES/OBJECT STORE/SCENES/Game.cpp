#include "customScenes.h"
#include "animations.h"
#include "postProcessManager.h"

IMAGE* shadow;
IMAGE* aimer;
IMAGE* pause;
IMAGE* minimap;

bool paused = false;
bool pauseReturn = false;

void GAME_SCENE::Init()
{

	name = "game";

	//ïœêî
	GAMEOBJECT* PlayerModel;
	GAMEOBJECT* map;
	ENEMY* enemy;

	//GAMEOBJECT
	gameManager = AddGameObject<EMPTYOBJECT>("GameManager");
	player = AddGameObject<PLAYER>("Player");
	PlayerModel = AddGameObject<PLAYERMODEL>("Player Model");
	enemy = AddGameObject<ENEMY>("Enemy");
	map = AddGameObject<EMPTYOBJECT>("Map");


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
	aimer = AddGameObject<IMAGE>("Aimer", SPRITE_LAYER);
	pause = AddGameObject<IMAGE>("Pause Menu", SPRITE_LAYER);
	minimap = AddGameObject<IMAGE>("Mini Map", SPRITE_LAYER);

	//ê⁄ë±èàóù
	{
		MainCamera->AddComponent<RevolutionCamera>();
		MainCamera->camera->SetTarget(player);

		PlayerModel->SetParent(player);
		player->GetComponent<PlayerMovement>()->gun1 = Manager::GetScene()->Find("Gun Left", LATEOBJECT_LAYER); 
		player->GetComponent<PlayerMovement>()->gun2 = Manager::GetScene()->Find("Gun Right", LATEOBJECT_LAYER); 
		player->GetComponent<PlayerMovement>()->model = PlayerModel->GetComponent<MeshFilter>();
		player->GetComponent<PlayerMovement>()->camera = MainCamera->camera;
		player->GetComponent<PlayerMovement>()->cameraController = MainCamera->GetComponent<RevolutionCamera>();

		reflectionProjector->transform->Position = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	}

	//ê›íË
	{
		gameManager->AddComponent<GameManager>();

		map->AddComponent<MeshFilter>()->SetModel(ModelReader::MAP_M);
		map->AddMaterial<FieldDefaultMaterial>();
		map->transform->culling = false;

		//UI
		aimer->AddComponent<ScreenToWorld>();
		aimer->GetMaterial()->SetTexture("_Texture", TextureReader::AIM_T);
		MainCamera->GetComponent<RevolutionCamera>()->aimer = aimer;

		pause->AddComponent<Animator>()->AddAnimation<PauseMenu>();
		pause->GetComponent<Animator>()->AddAnimation<PauseMenuReturn>();
		pause->GetComponent<Animator>()->SetUntimed(true);
		pause->GetComponent<Animator>()->SetCurrentIndex(0);

		minimap->AddMaterial<MiniMapMaterial>();

		player->GetComponent<PlayerMovement>()->aimPoint = aimer->GetComponent<ScreenToWorld>()->GetPoint();
	}

	//âπ
	{

		/*audio = enemy->AddComponent<AudioSource>();
		audio->SetClip(SoundReader::GAME);
		audio->SetThreeDimension(true);
		audio->SetPlayOnAwake(false);*/
		//audio->Play(true, 0.2f);
		AudioSource* as = MainCamera->AddComponent<AudioSource>();
		as->SetClip(SoundReader::BATTLE);
		as->SetPlayOnAwake(true);
		as->SetLoop(true);
		as->SetVolume(0.15f);
	}

	paused = false;
	pauseReturn = false;

	MainCamera->camera->SetLightDirection(D3DXVECTOR3(0.9f, -1.0f, -0.05f));

	player->AddComponent<AudioListener>(); //Listener
	player->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); // Roll Sound

	player->GetChildren()[0]->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Death Sound
	player->GetChildren()[0]->GetChildren()[0]->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Hit Sound

	player->GetComponent<PlayerMovement>()->gun1->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Gun Appear Sound
	player->GetComponent<PlayerMovement>()->gun2->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Gun Disappeat Sound

	player->GetComponent<PlayerMovement>()->gun1->GetChildren()[0]->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Shoot Sound
	player->GetComponent<PlayerMovement>()->gun2->GetChildren()[0]->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Shoot Sound
}

void GAME_SCENE::LateInit()
{
	//PostProcessManager::RemovePoster<DrunkMaterial>();
	PostProcessManager::AddPoster<PostProcessMaterial>();
	//Sound Components

	//Player
	/*
	player->AddComponent<AudioListener>(); //Listener
	player->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); // Roll Sound

	player->GetChildren()[0]->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Death Sound
	player->GetChildren()[0]->GetChildren()[0]->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Hit Sound

	player->GetComponent<PlayerMovement>()->gun1->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Gun Appear Sound
	player->GetComponent<PlayerMovement>()->gun2->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Gun Disappeat Sound

	player->GetComponent<PlayerMovement>()->gun1->GetChildren()[0]->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Shoot Sound
	player->GetComponent<PlayerMovement>()->gun2->GetChildren()[0]->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD); //Shoot Sound
	*/

	//Enemy
	//std::vector<ENEMY*> enemies = FindGameObjects<ENEMY>();
	//for (ENEMY* enemy : enemies)
	{
		//enemy->AddComponent<AudioSource>()->SetClip(SoundReader::GUARD);
		//enemy->GetComponent<AudioSource>()->SetThreeDimension(true);

		//Enemy Roam/Return Sound
		//Enemy Follow Sound
		//Enemy Attack Sound
		//Enemy Death Sound
		//Enemy Dance Sound
	}

	//Bullet
	//std::vector<BULLET*> bullets = FindGameObjects<BULLET>();
	//for (BULLET* bullet : bullets)
	{
		//Map hit
		//Enemy hit
	}

	//BGM
	//MainCamera->AddComponent<AudioSource>()->SetClip(SoundReader::GAME); // Game BGM
	// Title BGM
	// Death BGM
	// Pause On
	// Pause Off
	// Button Click
	// Button Change
	// Fade

	//Particle Systems
	{
		for (int layer = 0; layer < PARTICLE_EFFECT_MAX; layer++)
		{
			particleEffect[layer].clear();

			for (int i = 0; i < (layer != ENEMY_TO_PLAYER ? 50 : 10); i++)
			{
				std::string name = "effect_" + std::to_string((int)layer) + "_" + std::to_string(i);

				switch (layer)
				{
				case BULLET_TO_WALL:
					particleEffect[layer].push_back(/*Manager::GetDontDestroyOnLoadScene()->*/AddGameObject<BULLETDESTRUCTION>(name, BILLBOARD_LAYER));
				break;

				case BULLET_TO_ENEMY:
					particleEffect[layer].push_back(/*Manager::GetDontDestroyOnLoadScene()->*/AddGameObject<ENEMYDAMAGE>(name, BILLBOARD_LAYER));
				break;

				case ENEMY_TO_PLAYER:
					particleEffect[layer].push_back(/*Manager::GetDontDestroyOnLoadScene()->*/AddGameObject<PLAYERDAMAGE>(name, BILLBOARD_LAYER));
				break;

				default:
					break;

				}
			}
		}
	}

	//Enemy Color
	{
		std::vector<ENEMY*> enemies = FindGameObjects<ENEMY>();
		for (ENEMY* enemy : enemies)
		{
			int r, g, b;

			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;
			enemy->GetChildren()[0]->SetColor(D3DXCOLOR(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, 0.0f));

			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;
			enemy->GetChildren()[0]->GetMaterial()->SetColor("_Dissolve_Color", D3DXCOLOR(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, 1.0f));
		}

		srand(0);	//Seed Value for the random numbers

		for (ENEMY* enemy : enemies)
		{
			int a = rand() % 10;
			float scale = 1.3f + float(a) / 10.0f;
			enemy->transform->Scale = D3DXVECTOR3(scale, scale, scale);

			SphereCollider* collider = enemy->GetComponent<SphereCollider>();
			if (collider) 
			{ 
				collider->scaleOffset = 1.0f / enemy->transform->Scale.x;
			}

			collider = enemy->GetChildren()[0]->GetChildren()[0]->GetComponent<SphereCollider>();
			if (collider)
			{
				collider->scaleOffset = 1.0f / (enemy->transform->Scale.x * enemy->GetChildren()[0]->transform->Scale.x * enemy->GetChildren()[0]->GetChildren()[0]->transform->Scale.x);
			}

			EnemyHealth* health = enemy->GetComponent<EnemyHealth>();
			health->SetOffset(D3DXVECTOR3(health->GetOffset().x, 2.85f / 0.6f * scale, health->GetOffset().z));
			health->SetHealth(1000.0f / 0.6f * scale);
			health->GetHealthObject()->transform->Scale = D3DXVECTOR3(scale * 0.2f, scale * 0.5f / 3.0f, 0.1f);

			enemy->GetComponent<EnemyScript>()->SetAttack(50.0f / 0.6f * scale);
		}
	}

	pause->AddComponent<PauseMenuScript>();

	//if (GameObjects[FADE_LAYER].empty()) { GameObjects[FADE_LAYER].push_back(Manager::GetDontDestroyOnLoadScene()->Find("Fade")); }
}

void GAME_SCENE::Update()
{
	if (end == true && Fade->GetFadeIn() == false) { if (Fade->FadeOut() == false) { pause->GetComponent<PauseMenuScript>()->ChangeScene(); } }

	if (pause != nullptr)
	{
		Animator* pauser = pause->GetComponent<Animator>();
		if (pauser != nullptr)
		{
			if (Input::GetButtonTrigger(PAUSE_KEYMAP) && pauser->GetAnimationState(0) != Animation::PLAYBACK && pauser->GetAnimationState(1) != Animation::PLAYBACK)
			{
				if (paused == false)
				{
					Time::timeScale = 0.0f;
					pauser->PlayAnimation(0, Animation::PLAYBACK);
					paused = true;
					SoundReader::GetReadSound(SoundReader::PAUSE_IN)->Play(false, 0.2f);
					MainCamera->GetComponent<AudioSource>()->SetVolume(0.05f);
				}
				else
				{
					pauser->PlayAnimation(1, Animation::PLAYBACK);
					pauseReturn = true;
					SoundReader::GetReadSound(SoundReader::PAUSE_OUT)->Play(false, 0.2f);
				}
			}

			if (pauseReturn == true)
			{
				if (pauser->GetAnimationState(1) == Animation::END)
				{
					pauseReturn = false;
					paused = false;
					Time::timeScale = 1.0f;
					MainCamera->GetComponent<AudioSource>()->SetVolume(0.15f);
				}
			}
		}
	}

}

void GAME_SCENE::Resume()
{
	pause->GetComponent<Animator>()->PlayAnimation(1, Animation::PLAYBACK);
	pauseReturn = true;
	SoundReader::GetReadSound(SoundReader::PAUSE_OUT)->Play(false, 0.4f);
}