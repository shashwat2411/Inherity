#include "script.h"
#include "input.h"
#include "manager.h"

void EnemyScript::Start()
{
	gameObject->AddComponent<Rigidbody>()->useGravity = true;
	gameObject->AddComponent<SphereCollider>()->GetColliderObject()->transform->Position.y = 1.5f;
	gameObject->GetComponent<SphereCollider>()->SetCollisionSize(0.28f);
}

void EnemyScript::End()
{

}

void EnemyScript::Update()
{
	gameObject->transform->Position += gameObject->rigidbody->Speed * Time::fixedTimeScale;
	gameObject->rigidbody->Speed *= 0.9f;


	std::vector<BULLET*> bullets = Manager::GetScene()->FindGameObjects<BULLET>();

	for (GAMEOBJECT* bullet : bullets)
	{
		D3DXVECTOR3 distance;
		distance = bullet->transform->Position - gameObject->transform->Position;
		float length = D3DXVec3Length(&distance);

		if (length < gameObject->transform->Scale.x)
		{
			BulletScript* script = bullet->GetComponent<BulletScript>();
			if (script)
			{
				script->OnDestruction();
			}
		}
	}
}

void EnemyScript::Draw()
{

}

void EnemyScript::EngineDisplay()
{
	if (ImGui::TreeNode("Enemy Script"))
	{
		ImGui::TreePop();
		ImGui::Spacing();
	}
}
