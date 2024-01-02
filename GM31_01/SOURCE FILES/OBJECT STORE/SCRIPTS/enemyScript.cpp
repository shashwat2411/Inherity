#include "script.h"
#include "input.h"
#include "manager.h"

void EnemyScript::Start()
{
	gameObject->AddComponent<Rigidbody>()->useGravity = true;
	gameObject->GetChildren()[0]->AddComponent<SphereCollider>();
}

void EnemyScript::End()
{

}

void EnemyScript::Update()
{
	gameObject->transform->Position += gameObject->rigidbody->Speed * Time::fixedTimeScale;
	gameObject->rigidbody->Speed *= 0.9f;
}

void EnemyScript::Draw()
{

}

void EnemyScript::EngineDisplay()
{
	if (ImGui::TreeNode("Enemy Script"))
	{
		ImGui::TreePop();
		ImGui::Spacing();
	}
}
