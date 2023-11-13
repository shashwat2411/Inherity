#include "../OBJECT STORE/script.h"
#include "../MANAGEMENT FUNCTIONS/scene.h"
#include "../manager.h"

void BulletScript::Start()
{
	velocity = 0.4f;

	shooter = nullptr;

	gameObject->AddComponent<Rigidbody>();
	gameObject->AddComponent<MeshFilter>()->SetModel(ModelReader::GetReadModel(ModelReader::ENEMY_M));
}


void BulletScript::Update()
{
	if (counter < maxCounter) { counter += Time::deltaTime; }
	else
	{
		counter = 0.0f;
		gameObject->Destroy(true);
		return;
	}


	for (auto enemy : Manager::GetScene()->FindGameObjects<ENEMY>())
	{
		if (gameObject->transform->DistanceFrom(enemy) < 1.0f)
		{
			Manager::GetScene()->AddGameObject<EXPLOSION>(BILLBOARD_LAYER)->transform->Position = gameObject->transform->Position;
			gameObject->Destroy(true);
			//enemy->Destroy(true);
			return;
		}
	}

	if (shooter != nullptr)
	{
		gameObject->rigidbody->Speed = direction * velocity;
		gameObject->transform->Position += gameObject->rigidbody->Speed * Time::fixedTimeScale;
	}
}

void BulletScript::Shoot(GAMEOBJECT* st, float life, float speed, D3DXVECTOR3 offset)
{
	if (st == nullptr) { return; }

	maxCounter = life;

	velocity = speed;

	gameObject->transform->Position = offset;
	direction = shooter->transform->GetForwardDirection();

	shooter = st;
}

void BulletScript::OnCollisionEnter(GAMEOBJECT* obj)
{
	//Manager::GetScene()->AddGameObject<EXPLOSION>(BILLBOARD_LAYER)->transform->Position = transform->Position;
	//gameObject->Destroy(true);
	//enemy->Destroy(true);
	return;
}