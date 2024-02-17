#include "baseobjects.h"

void FADE::Init()
{
	Initialize();

	depth = false;
	fadeIn = true;
	fadeOut = true;

	alpha = 1.0f;
	speed = 0.02f; //0.05f

	Color = D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f);

	SetTag("Fade");
	
	AddComponent<SpriteRenderer>()->SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));

	AddMaterial<WipeMaterial>();
	GetMaterial()->SetTexture("_Texture", TextureReader::BLOOD_T);

	AddComponent<MiniMapVariable>();
}

void FADE::Update()
{
	if (fadeIn == true)
	{
		if (alpha > 0.0f) { alpha -= speed; }
		else { alpha = 0.0f; fadeIn = false; }

	}

	GetMaterial()->SetFloat("_Threshold", alpha);
}

bool FADE::FadeOut()
{
	fadeOut = true;

	if (alpha <= 1.0f) { alpha += speed; }
	else { alpha = 1.0f; fadeOut = false; }

	return fadeOut;
}