#include "script.h"
#include "manager.h"
#include "customScenes.h"

int pauseMenuSelection = 0;

void PauseMenuScript::Start()
{
	options[0] = Manager::GetScene()->AddGameObject<IMAGE>("Resume", SPRITE_LAYER);
	options[0]->GetMaterial()->SetTexture("_Texture", TextureReader::HOME2_T);
	options[0]->transform->Position.x = -30.0f;

	options[1] = Manager::GetScene()->AddGameObject<IMAGE>("Restart", SPRITE_LAYER);
	options[1]->GetMaterial()->SetTexture("_Texture", TextureReader::HOME2_T);
	options[1]->transform->Position.x = 0.0f;

	options[2] = Manager::GetScene()->AddGameObject<IMAGE>("Home", SPRITE_LAYER);
	options[2]->GetMaterial()->SetTexture("_Texture", TextureReader::HOME2_T);
	options[2]->transform->Position.x = 30.0f;

	originalSize = 0.4f;
	selectedSize = 0.5f;
	distance = 100.0f;

}


void PauseMenuScript::Draw()
{
	options[0]->transform->Position.x = gameObject->transform->Position.x - distance;
	options[1]->transform->Position.x = gameObject->transform->Position.x;
	options[2]->transform->Position.x = gameObject->transform->Position.x + distance;

	for (int i = 0; i < 3; i++)
	{
		options[i]->transform->Position.y = gameObject->transform->Position.y;
		options[i]->transform->Scale = originalSize * gameObject->transform->Scale;
	}

	options[pauseMenuSelection]->transform->Scale = selectedSize * gameObject->transform->Scale;


	if (Time::timeScale <= 0.0f)
	{
		GAME_SCENE* game = (GAME_SCENE*)Manager::GetScene();
		Animator* animator = gameObject->GetComponent<Animator>();
		if (animator->GetAnimationState(0) == Animation::END && animator->GetAnimationState(1) != Animation::PLAYBACK)
		{
			if (Input::GetButtonTrigger(LEFT_KEYMAP))
			{
				if (pauseMenuSelection > 0) { pauseMenuSelection--; }
				else { pauseMenuSelection = 2; }
				SoundReader::GetReadSound(SoundReader::OPTION_CHANGE)->Play(false, 0.4f);
			}
			else if (Input::GetButtonTrigger(RIGHT_KEYMAP))
			{
				if (pauseMenuSelection < 2) { pauseMenuSelection++; }
				else { pauseMenuSelection = 0; }
				SoundReader::GetReadSound(SoundReader::OPTION_CHANGE)->Play(false, 0.4f);
			}

			if (Input::GetButtonTrigger(CHANGE_KEYMAP))
			{
				if (pauseMenuSelection != 0) 
				{
					Time::timeScale = 1.0f;
					game->SetEnd(); 
				}
				else
				{
					game->Resume();
				}
			}
		}
	}
}

void PauseMenuScript::EngineDisplay()
{
	if (ImGui::TreeNode("Pause Menu Script"))
	{
		ImGui::SliderInt("Selected", &pauseMenuSelection, 0, 2);

		DebugManager::FloatDisplay(&originalSize, -FLT_MIN, "Original Size", true, D3DXVECTOR2(0.01f, 0.0f), 1);
		DebugManager::FloatDisplay(&selectedSize, -FLT_MIN, "Selected Size", true, D3DXVECTOR2(0.01f, 0.0f), 2);
		DebugManager::FloatDisplay(&distance, -FLT_MIN, "Distance", true, D3DXVECTOR2(0.01f, 0.0f), 3);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void PauseMenuScript::ChangeScene()
{
	if (pauseMenuSelection == 1) { Manager::ResetScene(); }
	else if(pauseMenuSelection == 2){ Manager::SetScene<TITLE_SCENE>(); }
}