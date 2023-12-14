#include "baseobjects.h"
#include "manager.h"
#include "textureReader.h"

void ParticleSystem::Start()
{
	burst = false;
	loop = true;
	rotateRandom = false;
	gravity = false;

	randomDirectionX = true;
	randomDirectionY = true;
	randomDirectionZ = true;

	numberOfObjects = 20;
	numberOfObjectsToAdd = 0;

	rotationSpeed = 0.0f;

	direction = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	scene = Manager::GetScene();


	//----------------------------------------------------------------
	texture = TextureReader::BOX_T;

	for (int i = 0; i < numberOfObjects; i++)
	{
		auto b = new PARTICLE();
		b->Init();

		b->GetMaterial()->SetTexture("_Texture", TextureReader::READ_TEXTURE(texture));
		objects.push_back(b);
	}

	for (auto object : objects)
	{
		object->SetParent(gameObject);
		object->ReInitialize();
		if (burst == true)
		{
			object->life = object->setLife;
			object->counter = 0.0f;
		}
	}
}

void ParticleSystem::End()
{
	for (auto object : objects)
	{
		object->UnInitialize();
		delete object;
	}
}

void ParticleSystem::Update()
{
	for (auto object : objects)
	{
		if (rotateRandom == true) { object->SetBillboard(false); object->transform->Rotation += object->velocity * 50.0f * Time::fixedTimeScale; }
		else { object->SetBillboard(true); }

		if (gravity == true)
		{
			D3DXVECTOR3 down = D3DXVECTOR3(0.0f, -(GRAVITY_CONSTANT * Time::deltaTime) * GRAVITY_ACCELERATION * 1.0f, 0.0f);
			D3DXVECTOR3 dir;
			D3DXVec3Normalize(&dir, &object->velocity);
			dir += down;
			D3DXVec3Normalize(&dir, &dir);
			object->velocity = dir * object->speed;
		}

		object->transform->Position += object->velocity * Time::fixedTimeScale;

		if (object->counter < object->life) { object->counter += Time::deltaTime; }
		else { object->counter = object->life; }

		if (object->counter >= object->life)
		{
			if (loop == true)
			{
				object->ReInitialize();
				if (burst == true)
				{
					object->life = object->setLife;
					object->counter = 0.0f;
				}
			}
			else
			{
				//gameObject->SetActive(false);
				//object->ReInitialize();
				//if (burst == true)
				//{
				//	object->life = object->setLife;
				//	object->counter = 0;
				//}
				//scene->GetCamera()->GetComponent<Camera>()->Target = nullptr;
			}
		}
	}
}

void ParticleSystem::Draw()
{
	for (auto object : objects)
	{
		object->Draw();
	}
}

void ParticleSystem::EngineDisplay()
{
	if (ImGui::TreeNode("Particle System"))
	{
		DebugManager::BoolDisplay(&loop, -200.0f, "Loop");
		DebugManager::BoolDisplay(&burst, -146.0f, "Burst", 1);

		ImGui::Text("\n");
		
		DebugManager::FloatDisplay(&rotationSpeed, -FLT_MIN, "Rotation Speed", true, D3DXVECTOR2(0.1f, 0.0f), 2);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void ParticleSystem::SetTexture(TextureReader::READ_TEXTURE text)
{
	for (PARTICLE* obj : objects)
	{
		obj->GetMaterial()->SetTexture("_Texture", text);
	}
	texture = text;
}

void ParticleSystem::SetLife(const float value)
{
	for (PARTICLE* obj : objects)
	{
		obj->life = obj->setLife = value;
	}
}

void ParticleSystem::SetSpeed(const float value)
{
	for (PARTICLE* obj : objects)
	{
		obj->speed = value;
	}
}

void ParticleSystem::SetDirection(D3DXVECTOR3 d)
{
	for (PARTICLE* obj : objects)
	{
		obj->direction = d;
	}
}

void ParticleSystem::SetRandomDirection(bool x, bool y, bool z)
{
	for (PARTICLE* obj : objects)
	{
		obj->randomDirectionX = x;
		obj->randomDirectionY = y;
		obj->randomDirectionZ = z;
	}
}

void ParticleSystem::Burst()
{
	if (gameObject != nullptr)
	{
		gameObject->SetActive(true);
		for (auto object : objects)
		{
			object->ReInitialize();
			if (burst == true)
			{
				object->life = object->setLife;
				object->counter = 0.0f;
			}
		}
	}
}

void ParticleSystem::SetParticleCount(int value)
{
	numberOfObjectsToAdd = value - numberOfObjects;

	while (numberOfObjectsToAdd > 0)
	{
		auto b = new PARTICLE();
		b->Init();

		b->GetMaterial()->SetTexture("_Texture", TextureReader::READ_TEXTURE(texture));
		objects.push_back(b);

		numberOfObjectsToAdd--;
		numberOfObjects++;
	}
	while (numberOfObjectsToAdd < 0)
	{
		objects.back()->UnInitialize();
		delete objects.back();
		objects.pop_back();

		numberOfObjectsToAdd++;
		numberOfObjects--;
	}
	for (auto object : objects)
	{
		object->SetParent(gameObject);
		object->ReInitialize();
		if (burst == true)
		{
			object->life = object->setLife;
			object->counter = 0.0f;
		}
	}
}