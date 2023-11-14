#include "script.h"
#include "manager.h"
#include "input.h"

void PlayerControl::Start()
{
	timerVector["_Counter"] = 0.0f;
}

void PlayerControl::End()
{

}

void PlayerControl::Update()
{

	if (Input::GetKeyPress('R'))
	{
		Spawn();
	}
}

void PlayerControl::Draw()
{

}

void PlayerControl::Spawn()
{
	if (Time::WaitForSeconds(2.0f, &timerVector["_Counter"]) == false) { return; }

	timerVector["_Counter"] = 0.0f;
	Manager::GetScene()->AddGameObject<ROCK>()->transform->Position = gameObject->transform->Position;
}