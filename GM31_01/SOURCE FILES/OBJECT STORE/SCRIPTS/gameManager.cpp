#include "script.h"
#include "manager.h"
#include "prefab.h"
#include "input.h"

void GameManager::Start()	
{
	slowMotionSpeed = 0.1f;

	Player = Manager::GetScene()->GetPlayer();

	gameObject->SetTag("GameManager");

}

void GameManager::End()
{

}

void GameManager::Update()
{
	if (Input::GetKeyPress(VK_LSHIFT))
	{
		Time::deltaTime = (1.0f / FRAME_RATE) * slowMotionSpeed;
		Time::fixedTimeScale = slowMotionSpeed;
	}
	else
	{
		Time::deltaTime = (1.0f / FRAME_RATE);
		Time::fixedTimeScale = 1.0f;
	}
}

void GameManager::Draw() 
{

}

void GameManager::EngineDisplay()
{
	if (ImGui::TreeNode("Game Manager"))
	{
		char str[24];

		ImGui::PushItemWidth(-FLT_MIN);
		sprintf_s(str, sizeof(str), "Slow Motion : %.2f", slowMotionSpeed);
		ImGui::SliderFloat(" ", &slowMotionSpeed, 0.0f, 2.0f, str);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}