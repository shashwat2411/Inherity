#include "../OBJECT STORE/prefab.h"

void BULLET::Start()
{
	Init();

	AddComponent<SphereCollider>()->SetCollisionSize(0.2f);
	AddComponent<BulletScript>();
	shadow = AddComponent<Shadow>();

	transform->Scale = D3DXVECTOR3(0.1f, 0.1f, 0.1f);

	shadow->shadow->transform->Scale = D3DXVECTOR3(0.9f, 0.9f, 0.9f);
}