#include "script.h"
#include "manager.h"
#include "customScenes.h"

void KnifeCollision::Start()
{

}


void KnifeCollision::Update()
{

}

void KnifeCollision::EngineDisplay()
{
	if (ImGui::TreeNode("Knife Collision"))
	{
		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void KnifeCollision::OnCollisionEnter(GAMEOBJECT* obj)
{
	if (obj->GetTag() == "Damager" && gameObject->Parent->Parent->GetComponent<ArtificialIntelligence>()->GetState() == ArtificialIntelligence::ATTACK)
	{
		Manager::GetScene()->GetPlayer()->GetComponent<PlayerHealth>()->Damage(gameObject->Parent->Parent->GetComponent<EnemyScript>()->GetAttack());
	}
}