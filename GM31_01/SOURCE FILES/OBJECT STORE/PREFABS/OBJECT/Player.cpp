#include "prefab.h"

void PLAYER::Start()
{
	Init();

	//AddComponent<SphereCollider>();
	AddComponent<PlayerMovement>();
	AddComponent<PlayerControl>();
	//AddComponent<Ground>();

	freezeY = true;

	sprintf(tag, "player");

	transform->Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}