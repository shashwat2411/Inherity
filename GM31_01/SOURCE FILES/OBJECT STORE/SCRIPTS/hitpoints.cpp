#include "../OBJECT STORE/script.h"

void HitPoints::Start()
{
	disable = false;
	invincible = false;
	noDamage = false;

	invincibilityCounter = 0;

	hp = 5.0f;

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
		if (invincibilityCounter < COLLIDE_COUNTDOWN) { invincibilityCounter++; }
		else { invincibilityCounter = 0; invincible = false; }
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