#include "baseobjects.h"

void POSTPROCESS::Init()
{
	Initialize();

	sprite = AddComponent<SpriteRenderer>();

	sprite->SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	AddMaterial<PostProcessMaterial>();

}

void POSTPROCESS::Update()
{

}