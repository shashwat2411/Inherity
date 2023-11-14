#include "component.h"
#include "input.h"

void OpenWorldCamera::Update()
{
	gameObject->transform->Position = Target->transform->Position + D3DXVECTOR3(0.0f, 5.0, 0.0f);
	gameObject->transform->Position += D3DXVECTOR3(cosf(gameObject->transform->Rotation.y), 0.0f, sinf(gameObject->transform->Rotation.y)) * 5.0f;

	if (Input::GetKeyPress('O'))
	{
		gameObject->transform->Rotation.y += 0.1f;
	}
	else if (Input::GetKeyPress('L'))
	{
		gameObject->transform->Rotation.y -= 0.1f;
	}
	//Disable Look At
}