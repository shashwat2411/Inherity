#include "../OBJECT STORE/script.h"

void EnemyScript::Start()
{
	gameObject->AddComponent<Rigidbody>();
	gameObject->AddComponent<SphereCollider>();

	//gameObject->AddMaterial<Default>();
}

void EnemyScript::End()
{

}

void EnemyScript::Update()
{
	gameObject->transform->Position += gameObject->rigidbody->Speed;
	gameObject->rigidbody->Speed *= 0.9f;
}

void EnemyScript::Draw()
{
}
