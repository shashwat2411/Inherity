#include "../OBJECT STORE/component.h"
#include "../OBJECT STORE/baseobjects.h"
#include "../manager.h"
#include "../MANAGEMENT FUNCTIONS/scene.h"

void ParticleSystem::Start()
{
	burst = false;
	loop = true;
	rotateRandom = false;

	numberOfObjects = 20;
	numberOfObjectsToAdd = 0;

	rotationSpeed = 0.0f;

	direction = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	scene = Manager::GetScene();


	//----------------------------------------------------------------
	texture = TextureReader::GetReadTexture(TextureReader::BOX_T);

	for (int i = 0; i < numberOfObjects; i++)
	{
		auto b = new PARTICLE();
		b->Start();

		b->GetMaterial()->SetTexture("_Texture", texture);
		objects.push_back(b);
	}

	for (auto object : objects)
	{
		object->SetParent(gameObject);
		object->ReInitialize();
		if (burst == true)
		{
			object->life = object->setLife;
			object->counter = 0;
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
		if (gameObject->GetActive() == true)
		{
			if (rotateRandom == true) { object->SetBillboard(false); object->transform->Rotation += object->velocity * 50.0f; }
			else { object->SetBillboard(true); }

			object->transform->Position += object->velocity;

			if (object->counter < object->life) { object->counter += 1; }
			else { object->counter = object->life; }

			if (object->counter == object->life)
			{
				if (loop == true)
				{
					object->ReInitialize();
					if (burst == true)
					{
						object->life = object->setLife;
						object->counter = 0;
					}
				}
				else
				{
					gameObject->SetActive(false);
					object->ReInitialize();
					if (burst == true)
					{
						object->life = object->setLife;
						object->counter = 0;
					}
					//scene->GetCamera()->GetComponent<Camera>()->Target = nullptr;
				}
			}
		}
	}

	//Necessary Stuff
	{
		/*if (Input::GetKeyTrigger('1')) { numberOfObjectsToAdd++; }
		if (Input::GetKeyTrigger('2')) { numberOfObjectsToAdd--; }

		while (numberOfObjectsToAdd > 0)
		{
			auto b = new PARTICLE();
			b->Start();
			b->image->SetTextureName(texture);
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
		}*/
	}

}

void ParticleSystem::Draw()
{
	for (auto object : objects)
	{
		object->Draw();
	}
}

void ParticleSystem::SetTexture(ID3D11ShaderResourceView* text)
{
	texture = text;
	for (PARTICLE* obj : objects)
	{
		obj->GetMaterial()->SetTexture("_Texture", texture);
	}
}

void ParticleSystem::SetLife(const int value)
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
				object->counter = 0;
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
		b->Start();

		b->GetMaterial()->SetTexture("_Texture", texture);
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
			object->counter = 0;
		}
	}
}