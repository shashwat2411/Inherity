#include "script.h"
#include "manager.h"
#include "customScenes.h"

int pauseMenuSelection = 0;

void PauseMenuScript::Start()
{
	pressed = false;

	options[0] = Manager::GetScene()->AddGameObject<IMAGE>("Resume", SPRITE_LAYER);
	options[0]->GetMaterial()->SetTexture("_Texture", TextureReader::RESUME_BUTTON_T);
	options[0]->transform->Position.x = -30.0f;

	options[1] = Manager::GetScene()->AddGameObject<IMAGE>("Restart", SPRITE_LAYER);
	options[1]->GetMaterial()->SetTexture("_Texture", TextureReader::RELOAD_BUTTON_T);
	options[1]->transform->Position.x = 0.0f;

	options[2] = Manager::GetScene()->AddGameObject<IMAGE>("Home", SPRITE_LAYER);
	options[2]->GetMaterial()->SetTexture("_Texture", TextureReader::HOME_BUTTON_T);
	options[2]->transform->Position.x = 30.0f;

	originalSize = 0.4f;
	selectedSize = 0.5f;
	distance = D3DXVECTOR3(100.0f, 45.0f, 0.0f);

	timerVector["pressCounter"] = 0.0f;

}


void PauseMenuScript::Draw()
{
	options[0]->transform->Position.x = gameObject->transform->Position.x - (distance.x * gameObject->transform->Scale.x);
	options[1]->transform->Position.x = gameObject->transform->Position.x;
	options[2]->transform->Position.x = gameObject->transform->Position.x + (distance.x * gameObject->transform->Scale.x);

	for (int i = 0; i < 3; i++)
	{
		options[i]->transform->Position.y = gameObject->transform->Position.y + (distance.y * gameObject->transform->Scale.y);
		options[i]->transform->Scale = originalSize * gameObject->transform->Scale;
	}

	options[pauseMenuSelection]->transform->Scale = selectedSize * gameObject->transform->Scale;


	if (Time::timeScale <= 0.0f)
	{
		GAME_SCENE* game = (GAME_SCENE*)Manager::GetScene();
		Animator* animator = gameObject->GetComponent<Animator>();
		if (animator->GetAnimationState(0) == Animation::END)
		{
			if (animator->GetAnimationState(1) != Animation::PLAYBACK)
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
					pressed = true;
					timerVector["pressCounter"] = 0.0f;
					SoundReader::GetReadSound(SoundReader::OPTION_SELECT_2)->Play(false, 0.4f);
				}
			}
		}

		if (pressed == true && Press())
		{
			if (pauseMenuSelection != 0)
			{
				Time::timeScale = 1.0f;
				game->SetEnd();
				pressed = false;
			}
			else
			{
				game->Resume();
				pressed = false;
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

		DebugManager::Float3Display(&distance, -1.0f, "Offset ", 0.05f, 3);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void PauseMenuScript::ChangeScene()
{
	if (Manager::GetScene()->GetPlayer()->GetComponent<PlayerHealth>()->GetDeath() == false)
	{
		if (pauseMenuSelection == 1) { Manager::ResetScene(); }
		else if (pauseMenuSelection == 2) { Manager::SetScene<TITLE_SCENE>(); }
	}
	else
	{
		Manager::SetScene<TITLE_SCENE>();
	}
}

bool PauseMenuScript::Press()
{
	if (Time::WaitForSeconds(0.1f, &timerVector["pressCounter"]) == false)
	{
		options[pauseMenuSelection]->transform->Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
		Input::SetControls(false);

		return false;
	}
	else if (Time::WaitForSeconds(0.5f, &timerVector["pressCounter"]) == false)
	{
		options[pauseMenuSelection]->transform->Scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
		Input::SetControls(true);
		return false;
	}

	return true;
}