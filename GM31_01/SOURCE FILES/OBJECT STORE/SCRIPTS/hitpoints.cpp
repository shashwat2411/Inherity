#include "script.h"

void HitPoints::Start()
{
	disable = false;
	invincible = false;
	noDamage = false;

	hp = 5.0f;
	timerVector["_Invincibility_Counter"] = 0.0f;

	barController = nullptr;

	object = nullptr;

}

void HitPoints::End()
{

}

void HitPoints::Update()
{
	if (barController != nullptr) { *barController = hp; }
	//if (hp <= 0.0f && noDamage == false)
	//{
	//	if (object != nullptr)
	//	{
	//		if (disable == true) { object->SetActive(false); }
	//		else { object->Destroy(); }
	//	}
	//	else
	//	{
	//		if (disable == true) { gameObject->SetActive(false); }
	//		else { gameObject->Destroy(); }
	//	}
	//}

	if (invincible == true)
	{
		if (timerVector["_Invincibility_Counter"] < COLLIDE_COUNTDOWN) { timerVector["_Invincibility_Counter"] += Time::deltaTime; }
		else { timerVector["_Invincibility_Counter"] = 0.0f; invincible = false; }
	}

	if (noDamage == true) { Heal(2.0f); }
}

void HitPoints::Draw()
{

}

bool HitPoints::Damage(float damage)
{
	if (invincible == false)
	{
		if ((hp - damage) > 0.0f) { hp -= damage; }
		else { hp = 0.0f; }

		invincible = true;
		
		return true;
	}

	return false;
}

void HitPoints::Heal(float value)
{
	if ((hp + value) < 100.0f) { hp += value; }
	else { hp = 100.0f; }
}