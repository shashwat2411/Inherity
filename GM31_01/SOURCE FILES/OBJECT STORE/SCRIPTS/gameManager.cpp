#include "script.h"
#include "manager.h"
#include "prefab.h"
#include "input.h"

void GameManager::Start()	
{
	slowMotionSpeed = 0.1f;

	Player = Manager::GetScene()->GetPlayer();

}

void GameManager::End()
{

}

void GameManager::Update()
{
	if (Input::GetKeyPress(VK_LSHIFT))
	{
		Time::deltaTime = (1.0f / GetFrameRate()) * slowMotionSpeed;
		Time::fixedTimeScale = (60.0f / GetFrameRate()) * slowMotionSpeed;
	}
	else
	{
		Time::deltaTime = (1.0f / GetFrameRate());
		Time::fixedTimeScale = (60.0f / GetFrameRate());
	}
}

void GameManager::Draw() 
{

}

void GameManager::EngineDisplay()
{
	if (ImGui::TreeNode("Game Manager"))
	{
		//char str[24];


		DebugManager::FloatDisplay(&slowMotionSpeed, -FLT_MIN, "Slow Motion", false, D3DXVECTOR2(0.0f, 2.0f));
		//ImGui::PushItemWidth(-FLT_MIN);
		//sprintf_s(str, sizeof(str), "Slow Motion : %.2f", slowMotionSpeed);
		//ImGui::SliderFloat(" ", &slowMotionSpeed, 0.0f, 2.0f, str);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}