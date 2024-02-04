#include "script.h"
#include "manager.h"

void PlayerHealth::Start()
{
	invincible = false;

	hp = 1000.0f;
	redHp = 1000.0f;
	maxHp = 1000.0f;

	timerVector["Invincibility Counter"] = 0.0f;
	timerVector["Bar Offset Multiplier"] = 300.0f;
	timerVector["Red HP Speed"] = 0.2f;

	health2 = Manager::GetScene()->AddGameObject<BAR>("Player HP Red", SPRITE_LAYER);
	health1 = Manager::GetScene()->AddGameObject<BAR>("Player HP", SPRITE_LAYER);

	health1->sprite->barOffsetLeft = 27.75f;
	health2->sprite->barOffsetLeft = 27.75f;
}

void PlayerHealth::End()
{

}

void PlayerHealth::Update()
{
	if (invincible == true)
	{
		if (timerVector["_Invincibility_Counter"] < COLLIDE_COUNTDOWN) { timerVector["_Invincibility_Counter"] += Time::deltaTime; }
		else { timerVector["_Invincibility_Counter"] = 0.0f; invincible = false; }
	}
}

void PlayerHealth::Draw()
{
	health2->transform->Position = health1->transform->Position;
	health2->transform->Scale = health1->transform->Scale;

	health1->sprite->barOffsetRight = hp / maxHp * timerVector["Bar Offset Multiplier"];
	health2->sprite->barOffsetRight = redHp / maxHp * timerVector["Bar Offset Multiplier"];

	redHp = Mathf::Lerp(redHp, hp, timerVector["Red HP Speed"] * Time::fixedTimeScale);
}

void PlayerHealth::EngineDisplay()
{
	if (ImGui::TreeNode("Player Health"))
	{
		DebugManager::FloatDisplay(&hp, -FLT_MIN, "HP", false, D3DXVECTOR2(0.0f, maxHp), 0);
		DebugManager::FloatDisplay(&maxHp, -FLT_MIN, "Max HP", true, D3DXVECTOR2(0.01f, 0.0f), 1);

		DebugManager::BoolDisplay(&invincible, -180.0f, "Invincible", 2);

		DebugManager::FloatDisplay(&timerVector["Bar Offset Multiplier"], -FLT_MIN, "Bar Offset Multiplier", true, D3DXVECTOR2(0.01f, 0.0f), 3);
		DebugManager::FloatDisplay(&timerVector["Red HP Speed"], -FLT_MIN, "Red HP Speed", true, D3DXVECTOR2(0.01f, 0.0f), 4);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

bool PlayerHealth::Damage(float damage)
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

void PlayerHealth::Heal(float value)
{
	if ((hp + value) < maxHp) { hp += value; }
	else { hp = maxHp; }
}