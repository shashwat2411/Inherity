#include "script.h"
#include "input.h"
#include "manager.h"

void EnemyScript::Start()
{
	death = false;

	gameObject->transform->drawRadius = 5.0f;

	gameObject->AddComponent<Rigidbody>()->useGravity = true;
	gameObject->AddComponent<SphereCollider>()->GetColliderObject()->transform->Position.y = 1.5f;
	gameObject->GetComponent<SphereCollider>()->SetCollisionSize(0.41f);

}

void EnemyScript::End()
{

}

void EnemyScript::Update()
{
	gameObject->transform->Position += gameObject->rigidbody->Speed * Time::fixedTimeScale;
	gameObject->rigidbody->Speed *= 0.9f;

	if (death == false)
	{
		std::vector<BULLET*> bullets = Manager::GetScene()->FindGameObjects<BULLET>();
		SphereCollider* collider = gameObject->GetComponent<SphereCollider>();

		for (GAMEOBJECT* bullet : bullets)
		{
			if (collider->GetColliderObject())
			{
				D3DXVECTOR3 distance;
				distance = bullet->transform->Position - collider->GetColliderObject()->transform->GlobalPosition;
				float length = D3DXVec3Length(&distance);

				if (length < collider->GetCollisionSize() + bullet->transform->Scale.x)
				{
					BulletScript* script = bullet->GetComponent<BulletScript>();
					if (script)
					{
						script->OnDestruction(true);
					}

					gameObject->GetComponent<EnemyHealth>()->Damage(1.0f);
				}
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

void EnemyScript::Death()
{
	if (death == false)
	{
		death = true;
		gameObject->GetComponent<ArtificialIntelligence>()->SetState(ArtificialIntelligence::DEATH);

		gameObject->RemoveComponent<EnemyHealth>();
		gameObject->RemoveComponent<SphereCollider>();
		gameObject->GetChildren()[0]->GetChildren()[0]->Destroy();
	}
}