#include "baseobjects.h"

void FADE::Init()
{
	Initialize();

	depth = false;
	fadeIn = true;
	fadeOut = false;
	soundPlay = false;

	alpha = 1.0f;
	speed = 0.04f; //0.05f

	Color = D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f);

	SetTag("Fade");
	
	//AddComponent<SpriteRenderer>()->SetSize(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	AddComponent<SpriteRenderer>()->SetSize(D3DXVECTOR2(16.0f, 27.0f));
	transform->Scale = D3DXVECTOR3(30.0f, 30.0f, 1.0f);
	transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

	AddMaterial<WipeMaterial>();
	GetMaterial()->SetTexture("_Texture", TextureReader::BLOOD_T);

	AddComponent<MiniMapVariable>();
}

void FADE::Update()
{
	if (fadeIn == true)
	{
		if (soundPlay == false) { soundPlay = true; SoundReader::GetReadSound(SoundReader::BLOOD_IN)->Play(false, 0.15f); }
		if (alpha > 0.01f) { alpha = Mathf::Lerp(alpha, 0.0f, speed); }
		else { alpha = 0.0f; fadeIn = false; soundPlay = false; }

	}

	GetMaterial()->SetFloat("_Threshold", alpha);
}

bool FADE::FadeOut()
{
	fadeOut = true;
	if (soundPlay == false) { soundPlay = true; SoundReader::GetReadSound(SoundReader::BLOOD_OUT)->Play(false, 0.2f); }

	if (alpha > -0.99f) { alpha = Mathf::Lerp(alpha, -1.0f, speed); }
	else { alpha = -1.0f; fadeOut = false; soundPlay = false; }

	return fadeOut;
}