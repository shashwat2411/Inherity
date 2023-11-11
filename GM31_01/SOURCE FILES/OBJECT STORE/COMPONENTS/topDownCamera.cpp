#include "../OBJECT STORE/component.h"

void TopDownCamera::Update()
{
	gameObject->transform->Position = Target->transform->Position + D3DXVECTOR3(0.0f, 5.0, -10.0f);
}