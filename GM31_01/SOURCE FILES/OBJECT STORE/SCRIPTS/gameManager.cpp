#include "script.h"
#include "manager.h"
#include "prefab.h"

void GameManager::Start()	
{
	Player = Manager::GetScene()->GetPlayer();
}

void GameManager::End()
{

}

void GameManager::Update()
{
	if (Input::GetKeyPress(VK_LSHIFT))
	{
		Time::deltaTime = (1.0f / FRAME_RATE) * 0.1f;
		Time::fixedTimeScale = 0.1f;
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