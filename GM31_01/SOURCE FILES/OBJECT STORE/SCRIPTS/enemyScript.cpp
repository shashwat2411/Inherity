#include "script.h"
#include "input.h"

void EnemyScript::Start()
{
	gameObject->AddComponent<Rigidbody>();
	gameObject->AddComponent<SphereCollider>();

	//gameObject->AddMaterial<DefaultMaterial>();
}

void EnemyScript::End()
{

}

void EnemyScript::Update()
{
	if (Input::GetKeyTrigger('V'))
	{
		gameObject->GetComponent<Animator>()->PlayAnimation(0);
	}

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
