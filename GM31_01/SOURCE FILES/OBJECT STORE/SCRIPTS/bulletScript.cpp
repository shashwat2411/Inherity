#include "script.h"
#include "manager.h"

void BulletScript::Start()
{
	gameObject->AddComponent<MeshFilter>()->SetModel(ModelReader::SPHERE_COLLIDER_M);
	gameObject->AddComponent<Rigidbody>();

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
		gameObject->Destroy();
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
	gameObject->Destroy();

	return;
}