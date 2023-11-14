#include "script.h"

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
	gameObject->transform->Position += gameObject->rigidbody->Speed * Time::fixedTimeScale;
	gameObject->rigidbody->Speed *= 0.9f;
}

void EnemyScript::Draw()
{
}
