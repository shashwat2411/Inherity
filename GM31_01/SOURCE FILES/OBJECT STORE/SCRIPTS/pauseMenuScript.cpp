#include "script.h"
#include "manager.h"

int pauseMenuSelection = 0;

void PauseMenuScript::Start()
{
	IMAGE* resume = Manager::GetScene()->AddGameObject<IMAGE>("Resume", SPRITE_LAYER);
	resume->GetMaterial()->SetTexture("_Texture", TextureReader::HOME2_T);
	resume->SetParent(gameObject);
	resume->transform->Position.x = -30.0f;

	resume = Manager::GetScene()->AddGameObject<IMAGE>("Restart", SPRITE_LAYER);
	resume->GetMaterial()->SetTexture("_Texture", TextureReader::HOME2_T);
	resume->SetParent(gameObject);
	resume->transform->Position.x = 0.0f;

	resume = Manager::GetScene()->AddGameObject<IMAGE>("Home", SPRITE_LAYER);
	resume->GetMaterial()->SetTexture("_Texture", TextureReader::HOME2_T);
	resume->SetParent(gameObject);
	resume->transform->Position.x = 30.0f;

	originalSize = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	selectedSize = D3DXVECTOR3(1.25f, 1.25f, 1.25f);
}


void PauseMenuScript::Draw()
{
	if (Time::timeScale <= 0.0f)
	{
		if (gameObject->GetComponent<Animator>()->GetAnimationState(0) == Animation::END)
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

			if (Manager::GetScene()->GetEnd() == false)
			{
				for (int i = 0; i < 3; i++)
				{
					gameObject->GetChildren()[i]->transform->Scale = originalSize;
				}

				gameObject->GetChildren()[pauseMenuSelection]->transform->Scale = selectedSize;
			}
		}
	}
}

void PauseMenuScript::EngineDisplay()
{
	if (ImGui::TreeNode("Pause Menu Script"))
	{
		DebugManager::Float3Display(&originalSize, -1.0f, "Original Size", 0.01f, 1);
		DebugManager::Float3Display(&selectedSize, -1.0f, "Selected Size", 0.01f, 2);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}