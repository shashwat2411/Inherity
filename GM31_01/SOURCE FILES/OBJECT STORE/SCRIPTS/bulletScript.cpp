#include "script.h"
#include "manager.h"
#include "customScenes.h"

void BulletScript::Start()
{
	destruction = false;

	gameObject->AddComponent<MeshFilter>()->SetModel(ModelReader::BULLET_M);
	gameObject->AddComponent<Rigidbody>();

	gameObject->AddMaterial<ToonMaterial>();

	gameObject->rigidbody->useGravity = false;
	gameObject->SetDepthShadow(false);

	gameObject->transform->Scale = D3DXVECTOR3(0.4f, 0.4, 0.4f);

	timerVector["counter"] = 0.0f;
	timerVector["max life"] = 3.0f;
}


void BulletScript::Update()
{
	timerVector["counter"] += Time::deltaTime;
	if (timerVector["counter"] >= timerVector["max life"] || gameObject->transform->Position.y < 0.0f)
	{
		OnDestruction(false);
	}

	gameObject->transform->Position += gameObject->rigidbody->Speed * Time::fixedTimeScale;
}

void BulletScript::EngineDisplay()
{
	if (ImGui::TreeNode("Bullet Script"))
	{
		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void BulletScript::OnCollisionEnter(GAMEOBJECT* obj)
{


	return;
}

void BulletScript::OnDestruction(bool enemyCollision)
{
	if (destruction == false)
	{
		destruction = true;

		GAME_SCENE* game = (GAME_SCENE*)Manager::GetScene();

		PARTICLESYSTEM* effect = game->GetParticleEffect((enemyCollision ? GAME_SCENE::BULLET_TO_ENEMY : GAME_SCENE::BULLET_TO_WALL));
		SoundReader::READ_SOUND sound = (enemyCollision ? SoundReader::ENEMY_HIT : SoundReader::GROUND_HIT);
		float volume = (enemyCollision ? 0.07f : 0.1f);

		SoundReader::GetReadSound(sound)->Play(false, volume);

		if (effect != nullptr && effect->particleSystem != nullptr)
		{
			effect->SetActive(true);
			effect->particleSystem->Play();
			effect->transform->Position = gameObject->transform->GlobalPosition;
		}

		gameObject->Destroy(true);
	}
}