#include "customScenes.h"

void TITLE_SCENE::Init()
{
	name = "title";

	//GAMEOBJECT

	//UI
	GAMEOBJECT* logo = AddGameObject<IMAGE>("Logo", SPRITE_LAYER);

	//eŽqŠÖŒWÝ’è

	//Ý’è
	{
		logo->GetMaterial()->SetTexture("_Texture", TextureReader::TITLE_BG_T);
		logo->GetComponent<SpriteRenderer>()->SetSize(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
		logo->transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	}

	//‰¹
	{
		SoundReader::GetReadSound(SoundReader::GAME)->Play(true, 0.2f);
	}
}

void TITLE_SCENE::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN)) { end = true; }
	if (end == true && Fade->GetFadeIn() == false) { if (Fade->FadeOut() == false) { Manager::SetScene<GAME_SCENE>(); } }
}