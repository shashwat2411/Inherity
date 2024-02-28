#include "script.h"
#include "manager.h"
#include "prefab.h"

void EnemyHealth::Start()
{
	hp = 1000.0f;
	redHp = 1000.0f;
	maxHp = 1000.0f;
	thicknessX = 0.02f;
	thicknessY = 0.1f;

	offset = D3DXVECTOR3(0.0f, 2.85f, 0.0f);

	timerVector["Red HP Speed"] = 0.06f;

	health = Manager::GetScene()->AddGameObject<BILLBOARD>("Enemy HP", BILLBOARD_LAYER);

	health->AddMaterial<GaugeMaterial>();
	health->billboard->SetSize(D3DXVECTOR2(5.0f, 1.0f));
	health->SetBillboard(true);
	health->GetMaterial()->SetTexture("_Texture", TextureReader::HEALTH_BAR_T);

	health->GetMaterial()->SetColor("_Base_Color", D3DXCOLOR(1.0f, 0.81f, 0.58f, 1.0f));
}

void EnemyHealth::End()
{
	health->Destroy();
}

void EnemyHealth::Update()
{

}

void EnemyHealth::Draw()
{
	health->transform->Position = gameObject->GetChildren()[0]->transform->GlobalPosition + offset;

	redHp = Mathf::Lerp(redHp, hp, timerVector["Red HP Speed"] * Time::fixedTimeScale);

	health->GetMaterial()->SetColor("_HP", D3DXCOLOR(hp, maxHp, redHp, 0.0f));
	health->GetMaterial()->SetFloat("_Thickness_X", thicknessX);
	health->GetMaterial()->SetFloat("_Thickness_Y", thicknessY);
}

void EnemyHealth::EngineDisplay()
{
	if (ImGui::TreeNode("Enemy Health"))
	{
		DebugManager::FloatDisplay(&hp, -FLT_MIN, "HP", false, D3DXVECTOR2(0.0f, maxHp), 0);
		DebugManager::FloatDisplay(&maxHp, -FLT_MIN, "Max HP", true, D3DXVECTOR2(0.01f, 0.0f), 1);
		DebugManager::FloatDisplay(&thicknessX, -FLT_MIN, "ThicknessX", true, D3DXVECTOR2(0.01f, 0.0f), 2);
		DebugManager::FloatDisplay(&thicknessY, -FLT_MIN, "ThicknessY", true, D3DXVECTOR2(0.01f, 0.0f), 3);

		DebugManager::FloatDisplay(&timerVector["Red HP Speed"], -FLT_MIN, "Red HP Speed", true, D3DXVECTOR2(0.01f, 0.0f), 4);

		DebugManager::Float3Display(&offset, -1.0f, "Offset ", 0.05f, 5);

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

void EnemyHealth::Damage(float damage)
{
	if ((hp - damage) > 0.0f) { hp -= damage; }
	else
	{
		hp = 0.0f;
		gameObject->GetComponent<EnemyScript>()->Death();
		SoundReader::GetReadSound(SoundReader::ENEMY_DEATH)->Play(false, 0.4f);
	}
}

void EnemyHealth::Heal(float value)
{
	if ((hp + value) < maxHp) { hp += value; }
	else { hp = maxHp; }
}