#include "component.h"
#include "input.h"

float rotation = 0.0f;

void RevolutionCamera::Update()
{

	if (Input::GetKeyPress('Q')) { rotation += 0.1f; }
	if (Input::GetKeyPress('E')) { rotation -= 0.1f; }

	gameObject->transform->Position = Target->transform->Position + D3DXVECTOR3(sinf(rotation) * 5.0f, 4.0f, cosf(rotation) * 5.0f);

}