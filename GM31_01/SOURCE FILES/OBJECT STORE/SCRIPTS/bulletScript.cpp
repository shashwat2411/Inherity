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

	gameObject->transform->Scale = D3DXVECTOR3(0.4f, 0.4, 0.4f);

	timerVector["counter"] = 0.0f;
	timerVector["max life"] = 4.0f;
}


void BulletScript::Update()
{
	timerVector["counter"] += Time::deltaTime;
	if (timerVector["counter"] >= timerVector["max life"])
	{
		OnDestruction();
	}

	//if (gameObject->transform->GlobalPosition.y < gameObject->transform->Scale.y / 2.0f)
	//{
	//	OnDestruction();
	//}

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

void BulletScript::OnDestruction()
{
	if (destruction == false)
	{
		destruction = true;

		GAME_SCENE* game = (GAME_SCENE*)Manager::GetScene();
		BULLETDESTRUCTION* effect = game->GetBulletDestroyEffect();

		if (effect != nullptr && effect->particleSystem != nullptr)
		{
			effect->SetActive(true);
			effect->particleSystem->Play();
			effect->transform->Position = gameObject->transform->GlobalPosition;
		}

		gameObject->Destroy(true);
	}
}