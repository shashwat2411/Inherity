#include "customScenes.h"

void RESULT_SCENE::Init()
{
	name = "result";

	//GAMEOBJECT

	//UI
	GAMEOBJECT* result = AddGameObject<IMAGE>("Result", SPRITE_LAYER);

	//親子関係設定
	{

	}

	//設定
	{
		result->GetMaterial()->SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::RESULT_BG_T));
		result->GetComponent<SpriteRenderer>()->SetSize(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
		result->transform->Position = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	}

	//音
	{
		SoundReader::GetReadSound(SoundReader::GAME)->Play(true, 0.2f);
	}
}

void RESULT_SCENE::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN)) { end = true; }
	if (end == true && Fade->GetFadeIn() == false) { if (Fade->FadeOut() == false) { Manager::SetScene<TITLE_SCENE>(); } }
}