#include "component.h"

void FirstPersonCamera::Update()
{
	gameObject->transform->Position = D3DXVECTOR3(0.0f, 4.0f, -8.0f);
	//Disable Look At
}

