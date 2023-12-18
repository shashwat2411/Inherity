#include "baseobjects.h"
#include "manager.h"
#include "textureReader.h"

BILLBOARD* part;

void ParticleSystem::Start()
{
	play = false;
	show = false;
	burst = false;
	loop = true;
	gravity = false;

	counter = 0.0f;
	life = 2.0f;

	randomVelocity = D3DXBOOL3(false, false, false);
	randomPosition = D3DXBOOL3(false, false, false);
	randomRotation = D3DXBOOL3(false, false, false);
	//randomDirectionX = true;
	//randomDirectionY = true;
	//randomDirectionZ = true;

	numberOfObjects = MAX_PARTICLES;
	numberOfObjectsToAdd = 0;

	size = D3DXVECTOR3(0.1f, 0.1f, 0.1f);
	positionOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	velocityOffset = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	rotationOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	speed = D3DXVECTOR3(0.1f, 0.1f, 0.1f);

	scene = Manager::GetScene();

	part = new BILLBOARD();
	part->Init();
	part->SetParent(gameObject);

	//----------------------------------------------------------------
	textureIndex = TextureReader::BOX_T;
	part->AddMaterial<GeometryInstancingMaterial>()->SetTexture("_Texture", TextureReader::HOME2_T);

	for (int i = 0; i < numberOfObjects; i++)
	{
		auto b = new PARTICLE();
		b->Init();

		b->GetMaterial()->SetTexture("_Texture", (TextureReader::READ_TEXTURE)textureIndex);
		particles.push_back(b);
	}

	for (auto object : particles)
	{
		object->SetParent(gameObject);

		Reinitialize(object);
	}
}

void ParticleSystem::End()
{
	for (auto object : particles)
	{
		object->UnInitialize();
		delete object;
	}
	part->UnInitialize();
	delete part;
}

void ParticleSystem::Update()
{
	if (play == true)
	{
		counter += Time::deltaTime;

		if (counter >= life)
		{
			counter = 0.0f;

			if (loop == true)
			{
				for (PARTICLE* p : particles) { Reinitialize(p); }
			}
			else
			{
				play = false;
				show = false;
			}
		}
	}

	if (show == true)
	{
		for (PARTICLE* p : particles)
		{
			p->transform->Scale = size;

			Run(p);
		}
	}

	part->Update();
}

void ParticleSystem::Draw()
{
	if (DebugManager::play == false)
	{
		if (play == true)
		{
			counter += Time::deltaTime;

			if (counter >= life)
			{
				counter = 0.0f;

				if (loop == true)
				{
					for (PARTICLE* p : particles) { Reinitialize(p); }
				}
				else
				{
					play = false;
					show = false;
				}
			}
		}
	}

	if (show == true)
	{
		for (PARTICLE* p : particles)
		{
			p->transform->Scale = size;

			if (DebugManager::play == false) { Run(p); }

			//p->Draw();
		}


		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(part->GetMaterial()->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		D3DXVECTOR3* vertex = (D3DXVECTOR3*)msr.pData;

		for (int x = 0; x < MAX_PARTICLES; x++)
		{
			vertex[x] = particles[x]->transform->Position;
		}

		Renderer::GetDeviceContext()->Unmap(part->GetMaterial()->GetBuffer(), 0);

		part->Draw();

	}
}

void ParticleSystem::EngineDisplay()
{
	if (ImGui::TreeNode("Particle System"))
	{
		if (DebugManager::BoolDisplay(&play, -200.0f, "Play", 0)) { if (show == false) { Play(); } }

		DebugManager::BoolDisplay(&loop, -200.0f, "Loop", 1);
		DebugManager::BoolDisplay(&burst, -146.0f, "Burst", 2);

		int num = numberOfObjects;
		if (ImGui::InputInt("Particles", &num))
		{
			SetParticleCount(num);
		}

		ImGui::Text("Texture : ");
		ImGui::SameLine();

		ImGui::PushID(3);
		ImGui::PushItemWidth(0.0f);
		if (ImGui::Combo("", &textureIndex, TextureReader::GetTextureNames(), TextureReader::READ_TEXTURE_MAX))
		{
			SetTexture((TextureReader::READ_TEXTURE)textureIndex);
		}
		ImGui::PopID();

		ImGui::Checkbox("Gravity", &gravity);
		ImGui::DragFloat("Life", &life, 0.01f);

		ImGui::PushID(4);
		ImGui::BeginGroup();
		ImGui::Text("Random Velocity");
		ImGui::Checkbox("X", &randomVelocity.x); ImGui::SameLine();
		ImGui::Checkbox("Y", &randomVelocity.y); ImGui::SameLine();
		ImGui::Checkbox("Z", &randomVelocity.z);
		ImGui::Text("\n");
		ImGui::EndGroup();
		ImGui::PopID();

		ImGui::PushID(5);
		ImGui::BeginGroup();
		ImGui::Text("Random Position");
		ImGui::Checkbox("X", &randomPosition.x); ImGui::SameLine();
		ImGui::Checkbox("Y", &randomPosition.y); ImGui::SameLine();
		ImGui::Checkbox("Z", &randomPosition.z);
		ImGui::Text("\n");
		ImGui::EndGroup();
		ImGui::PopID();

		ImGui::PushID(6);
		ImGui::BeginGroup();
		ImGui::Text("Random Rotation");
		ImGui::Checkbox("X", &randomRotation.x); ImGui::SameLine();
		ImGui::Checkbox("Y", &randomRotation.y); ImGui::SameLine();
		ImGui::Checkbox("Z", &randomRotation.z);
		ImGui::Text("\n");
		ImGui::EndGroup();
		ImGui::PopID();

		ImGui::PushID(7);
		ImGui::BeginGroup();
		DebugManager::Float3Display(&size, -1.0f, "Size ", 0.005f, 0);

		ImGui::SeparatorText("Offset");
		DebugManager::Float3Display(&velocityOffset, -1.0f, "Velocity ", 0.01f, 1, false, -1.0f, 1.0f);
		DebugManager::Float3Display(&positionOffset, -1.0f, "Position ", 0.01f, 2);
		DebugManager::Float3Display(&rotationOffset, -1.0f, "Rotation ", 0.01f, 3);
		ImGui::EndGroup();
		ImGui::PopID();

		ImGui::Text("\n");

		ImGui::PushID(8);
		ImGui::BeginGroup();
		DebugManager::Float3Display(&speed, -1.0f, "Speed ", 0.01f, 0);
		ImGui::EndGroup();
		ImGui::PopID();

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void ParticleSystem::SetTexture(TextureReader::READ_TEXTURE text)
{
	textureIndex = text;
	for (PARTICLE* p : particles)
	{
		p->GetMaterial()->SetTexture("_Texture", (TextureReader::READ_TEXTURE)textureIndex);
	}
}

void ParticleSystem::SetParticleCount(int value)
{
	numberOfObjectsToAdd = value - numberOfObjects;

	while (numberOfObjectsToAdd > 0)
	{
		auto b = new PARTICLE();
		b->Init();

		b->GetMaterial()->SetTexture("_Texture", (TextureReader::READ_TEXTURE)textureIndex);
		particles.push_back(b);

		numberOfObjectsToAdd--;
		numberOfObjects++;
	}
	while (numberOfObjectsToAdd < 0)
	{
		particles.back()->UnInitialize();
		delete particles.back();
		particles.pop_back();

		numberOfObjectsToAdd++;
		numberOfObjects--;
	}
	for (auto object : particles)
	{
		object->SetParent(gameObject);

		Reinitialize(object);
	}
}

void ParticleSystem::Reinitialize(PARTICLE* p)
{
	if (p != nullptr)
	{
		p->counter = 0.0f;

		p->direction = velocityOffset;
		p->transform->Position = positionOffset;
		p->rotationVelocity = rotationOffset;

		//Position
		if (randomPosition == true)
		{
			D3DXVECTOR3 rp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			D3DXVECTOR3 pos = (positionOffset * 100.0f) + D3DXVECTOR3(1.0f, 1.0f, 1.0f);

			int a = (int)pos.x;
			int b = (int)pos.y;
			int c = (int)pos.z;

			a = (a == 0 ? 1 : a);
			b = (b == 0 ? 1 : b);
			c = (c == 0 ? 1 : c);

			if (randomPosition.x == true) { rp.x = rand() % a; }
			if (randomPosition.y == true) { rp.y = rand() % b; }
			if (randomPosition.z == true) { rp.z = rand() % c; }
			int dirx = rand() % 2;
			int diry = rand() % 2;
			int dirz = rand() % 2;
			if (dirx == 1 && randomPosition.x == true) { rp.x *= -1.0f; }
			if (diry == 1 && randomPosition.y == true) { rp.y *= -1.0f; }
			if (dirz == 1 && randomPosition.z == true) { rp.z *= -1.0f; }

			rp /= 100.0f;

			p->transform->Position = rp;
		}

		if (randomVelocity == true)
		{
			D3DXVECTOR3 d = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			D3DXVECTOR3 vel = (velocityOffset * 100.0f) + D3DXVECTOR3(1.0f, 1.0f, 1.0f);

			int a = (int)vel.x;
			int b = (int)vel.y;
			int c = (int)vel.z;

			a = (a == 0 ? 1 : a);
			b = (b == 0 ? 1 : b);
			c = (c == 0 ? 1 : c);

			if (randomVelocity.x == true) { d.x = rand() % a; }
			if (randomVelocity.y == true) { d.y = rand() % b; }
			if (randomVelocity.z == true) { d.z = rand() % c; }
			int dirx = rand() % 2;
			int diry = rand() % 2;
			int dirz = rand() % 2;
			if (dirx == 1 && randomVelocity.x == true) { d.x *= -1.0f; }
			if (diry == 1 && randomVelocity.y == true) { d.y *= -1.0f; }
			if (dirz == 1 && randomVelocity.z == true) { d.z *= -1.0f; }
			D3DXVec3Normalize(&d, &d);

			p->direction = d;
		}

		if (randomRotation == true)
		{
			p->SetBillboard(false);

			D3DXVECTOR3 rr = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (randomRotation.x == true) { rr.x = rand() % (int)rotationOffset.x; }
			if (randomRotation.y == true) { rr.y = rand() % (int)rotationOffset.y; }
			if (randomRotation.z == true) { rr.z = rand() % (int)rotationOffset.z; }
			int dirx = rand() % 2;
			int diry = rand() % 2;
			int dirz = rand() % 2;
			if (dirx == 1 && randomRotation.x == true) { rr.x *= -1.0f; }
			if (diry == 1 && randomRotation.y == true) { rr.y *= -1.0f; }
			if (dirz == 1 && randomRotation.z == true) { rr.z *= -1.0f; }

			p->rotationVelocity = rr;
		}
		else { p->SetBillboard(true); }

		if (burst == false) { p->life = (float)(rand() % ((int)FRAME_RATE + 1)) / FRAME_RATE; }
		else { p->life = life; }
	}
	else
	{
		for (PARTICLE* particle : particles)
		{
			Reinitialize(particle);
		}
	}
}

void ParticleSystem::Run(PARTICLE* p)
{
	//Gravity
	if (gravity == true)
	{
		D3DXVECTOR3 down = D3DXVECTOR3(0.0f, -(GRAVITY_CONSTANT * Time::deltaTime) * GRAVITY_ACCELERATION * 1.0f, 0.0f);
		D3DXVECTOR3 dir = p->direction + down;
		D3DXVec3Normalize(&dir, &dir);

		p->direction = dir;
	}

	//Value Update
	D3DXVECTOR3 v;
	v.x = p->direction.x * speed.x;
	v.y = p->direction.y * speed.y;
	v.z = p->direction.z * speed.z;

	p->transform->Position += v * Time::fixedTimeScale;
	p->transform->Rotation += p->rotationVelocity * Time::fixedTimeScale;

	//Life
	p->counter += Time::deltaTime;
	if (p->counter >= p->life && burst == false)
	{
		if (loop == true)
		{
			Reinitialize(p);
		}
	}


	//if (burst == false)
	//{
	//	if (p->counter < p->life) { p->counter += Time::deltaTime; }
	//	else { if (loop == true) { Reinitialize(p); } }
	//}
	//else
	//{
	//	if (counter < life) { counter += Time::deltaTime; }
	//	else { if (loop == true) { Reinitialize(p); } }
	//}
}

void ParticleSystem::Play()
{
	play = true;
	show = true;
	counter = 0.0f;
	for (PARTICLE* p : particles) { Reinitialize(p); }
}


template<class Archive>
void ParticleSystem::serialize(Archive & archive)
{
	int particleNumber;
	if (Archive::is_saving::value)
	{
		particleNumber = numberOfObjects;
	}

	archive(cereal::virtual_base_class<Component>(this),
		CEREAL_NVP(textureIndex),
		cereal::make_nvp("numberOfObjects", particleNumber),
		CEREAL_NVP(burst),
		CEREAL_NVP(loop),
		CEREAL_NVP(gravity),
		CEREAL_NVP(life),
		CEREAL_NVP(randomVelocity),
		CEREAL_NVP(randomPosition),
		CEREAL_NVP(randomRotation),
		CEREAL_NVP(size),
		CEREAL_NVP(positionOffset),
		CEREAL_NVP(velocityOffset),
		CEREAL_NVP(rotationOffset),
		CEREAL_NVP(speed)
	);

	if (Archive::is_loading::value)
	{
		SetParticleCount(particleNumber);
		for(PARTICLE* p : particles)
		{
			p->GetMaterial()->SetTexture("_Texture", ((TextureReader::READ_TEXTURE)textureIndex));
			Reinitialize(p);
		}
	}
}

template void ParticleSystem::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive& archive);
template void ParticleSystem::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive& archive);