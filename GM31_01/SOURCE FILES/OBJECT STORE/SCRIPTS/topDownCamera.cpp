#include "script.h"

void TopDownCamera::Update()
{
	gameObject->transform->Position = camera->Target->transform->Position + D3DXVECTOR3(0.0f, 5.0, -10.0f);
}