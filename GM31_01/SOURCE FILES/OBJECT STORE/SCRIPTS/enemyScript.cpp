#include "script.h"
#include "input.h"
#include "manager.h"

void EnemyScript::Start()
{
	death = false;

	gameObject->transform->drawRadius = 5.0f;

	gameObject->AddComponent<Rigidbody>()->useGravity = true;

	SphereCollider* collider = gameObject->AddComponent<SphereCollider>();
	collider->GetColliderObject()->transform->Position.y = 1.5f;
	collider->SetCollisionSize(0.41f);

	collider->ClearCollisionLayer();
	collider->SetSelfLayer(COLLISION_LAYER::ENEMY_CL);
	collider->AddCollisionLayer(COLLISION_LAYER::PLAYER_CL);
	collider->AddCollisionLayer(COLLISION_LAYER::ENEMY_CL);

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
		D3DXVECTOR3 displacement = gameObject->transform->Position - Manager::GetScene()->GetPlayer()->transform->Position;
		float distance = D3DXVec3Length(&displacement);
		if (distance < 80.0f)
		{
			gameObject->GetChildren()[0]->GetComponent<MeshFilter>()->SetDraw(true);

			std::vector<BULLET*> bullets = Manager::GetScene()->FindGameObjects<BULLET>();
			SphereCollider* collider = gameObject->GetComponent<SphereCollider>();

			if (collider->GetColliderObject())
			{
				for (GAMEOBJECT* bullet : bullets)
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

						EnemyHealth* health = gameObject->GetComponent<EnemyHealth>();
						if (health) 
						{ 
							health->Damage(8.0f);

							GAMEOBJECT* child = gameObject->GetChildren()[0];
							//child->GetComponent<MeshFilter>()->SetAnimationBlend("Enemy_Damage", false, 0.001f);
							child->SetColor(D3DXCOLOR(child->GetColor().r, child->GetColor().g, child->GetColor().b, child->GetColor().a + 0.75f));

							gameObject->GetComponent<ArtificialIntelligence>()->SetStateToFollow();
						}
					}
				}
			}
		}

		if (distance > 140.0f) { gameObject->GetChildren()[0]->GetComponent<MeshFilter>()->SetDraw(false); }
		else{ gameObject->GetChildren()[0]->GetComponent<MeshFilter>()->SetDraw(true); }
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
		//SoundReader::GetReadSound(SoundReader::ENEMY_DEATH)->Play(false, 0.2f);

		death = true;
		gameObject->GetComponent<ArtificialIntelligence>()->SetState(ArtificialIntelligence::DEATH);

		gameObject->RemoveComponent<EnemyHealth>();
		gameObject->RemoveComponent<SphereCollider>();
		gameObject->GetChildren()[0]->GetChildren()[0]->Destroy();
	}
}