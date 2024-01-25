#include "prefab.h"

void BULLET::Start()
{
	Init();

	speed = 1.0f;

	AddComponent<BulletScript>();
}