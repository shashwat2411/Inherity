#include "prefab.h"

void PLAYER::Start()
{
	Init();

	//AddComponent<SphereCollider>();
	AddComponent<PlayerMovement>();
	AddComponent<PlayerControl>();
	AddComponent<PlayerHealth>();
	//AddComponent<Ground>();

	freezeY = true;

	transform->Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}