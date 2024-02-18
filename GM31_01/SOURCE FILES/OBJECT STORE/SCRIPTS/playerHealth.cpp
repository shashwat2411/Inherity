#include "script.h"
#include "manager.h"
#include "customScenes.h"

void PlayerHealth::Start()
{
	invincible = false;

	hp = 1000.0f;
	redHp = 1000.0f;
	maxHp = 1000.0f;
	thicknessX = 0.02f;
	thicknessY = 0.16f;

	timerVector["Invincibility Counter"] = 0.0f;
	timerVector["Bar Offset Multiplier"] = 300.0f;
	timerVector["Red HP Speed"] = 0.06f;

	health = Manager::GetScene()->AddGameObject<BAR>("Player HP", SPRITE_LAYER);

	health->sprite->barOffsetLeft = 27.75f;

	health->AddMaterial<GaugeMaterial>();
}

void PlayerHealth::End()
{

}

void PlayerHealth::Update()
{
	if (invincible == true)
	{
		if (timerVector["_Invincibility_Counter"] < (30.0f / FRAME_RATE)) { timerVector["_Invincibility_Counter"] += Time::deltaTime; }
		else { timerVector["_Invincibility_Counter"] = 0.0f; invincible = false; }
	}
}

void PlayerHealth::Draw()
{
	health->sprite->barOffsetRight = /*hp / maxHp * */timerVector["Bar Offset Multiplier"];

	redHp = Mathf::Lerp(redHp, hp, timerVector["Red HP Speed"] * Time::fixedTimeScale);

	health->GetMaterial()->SetColor("_HP", D3DXCOLOR(hp, maxHp, redHp, 0.0f));
	health->GetMaterial()->SetFloat("_Thickness_X", thicknessX);
	health->GetMaterial()->SetFloat("_Thickness_Y", thicknessY);
}

void PlayerHealth::EngineDisplay()
{
	if (ImGui::TreeNode("Player Health"))
	{
		DebugManager::FloatDisplay(&hp, -FLT_MIN, "HP", false, D3DXVECTOR2(0.0f, maxHp), 0);
		DebugManager::FloatDisplay(&maxHp, -FLT_MIN, "Max HP", true, D3DXVECTOR2(0.01f, 0.0f), 1);
		DebugManager::FloatDisplay(&thicknessX, -FLT_MIN, "ThicknessX", true, D3DXVECTOR2(0.01f, 0.0f), 2);
		DebugManager::FloatDisplay(&thicknessY, -FLT_MIN, "ThicknessY", true, D3DXVECTOR2(0.01f, 0.0f), 3);

		DebugManager::BoolDisplay(&invincible, -180.0f, "Invincible", 4);

		DebugManager::FloatDisplay(&timerVector["Bar Offset Multiplier"], -FLT_MIN, "Bar Offset Multiplier", true, D3DXVECTOR2(0.01f, 0.0f), 5);
		DebugManager::FloatDisplay(&timerVector["Red HP Speed"], -FLT_MIN, "Red HP Speed", true, D3DXVECTOR2(0.01f, 0.0f), 6);

		D3DXCOLOR c1 = health->GetMaterial()->GetColor("_Base_Color");
		D3DXCOLOR c2 = health->GetMaterial()->GetColor("_Lost_Color");
		D3DXCOLOR c3 = health->GetMaterial()->GetColor("_Difference_Color");

		ImGui::ColorEdit4("Base Color", c1);
		ImGui::ColorEdit4("Lost Color", c2);
		ImGui::ColorEdit4("Difference Color", c3);

		health->GetMaterial()->SetColor("_Base_Color", c1);
		health->GetMaterial()->SetColor("_Lost_Color", c2);
		health->GetMaterial()->SetColor("_Difference_Color", c3);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

bool PlayerHealth::Damage(float damage)
{
	if (invincible == false)
	{
		GAME_SCENE* game = (GAME_SCENE*)Manager::GetScene();
		PARTICLESYSTEM* effect = game->GetParticleEffect(GAME_SCENE::ENEMY_TO_PLAYER);
		PlayerMovement* playerMove = gameObject->GetComponent<PlayerMovement>();

		if (playerMove->GetInvincibility() == true) 
		{
			return false; 
		}

		if (effect != nullptr && effect->particleSystem != nullptr)
		{
			effect->SetActive(true);
			effect->particleSystem->Play();
			effect->transform->Position = gameObject->transform->GlobalPosition;
		}

		Manager::GetScene()->GetCamera()->camera->CameraShake(D3DXVECTOR3(1.0f, 0.0f, 0.0f), 1.0f);

		if ((hp - damage) > 0.0f) 
		{ 
			hp -= damage;
			playerMove->SetState(PlayerMovement::HIT_PS);
			invincible = true;
		}
		else
		{
			hp = 0.0f;
			playerMove->SetState(PlayerMovement::DEATH_PS);

			std::vector<ENEMY*> enemies = Manager::GetScene()->FindGameObjects<ENEMY>();
			for (ENEMY* enemy : enemies)
			{
				enemy->GetComponent<ArtificialIntelligence>()->Dancing();
			}
		}

		return true;
	}

	return false;
}

void PlayerHealth::Heal(float value)
{
	if ((hp + value) < maxHp) { hp += value; }
	else { hp = maxHp; }
}