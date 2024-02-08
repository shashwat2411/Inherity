#include "prefab.h"
#include "manager.h"

void PLAYERDAMAGE::Start()
{
	Init();

	depthShadow = false;
	active = false;
	save = false;

	particleSystem->size = D3DXVECTOR3(3.0f, 3.0f, 3.0f);

	particleSystem->SetParticleCount(25);
	particleSystem->burst = true;
	particleSystem->loop = false;
	particleSystem->SetPlay(false);

	particleSystem->life = 2.0f;

	particleSystem->SetTexture(TextureReader::EMPTY_T);

	particleSystem->gravity = true;
	particleSystem->randomVelocity = D3DXBOOL3(true, true, true);

	particleSystem->speed = D3DXVECTOR3(4.0f, 4.0f, 4.0f);

	transform->Scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);

	billboard = true;

}

void PLAYERDAMAGE::Update()
{
	if (particleSystem->GetPlay() == false)
	{
		active = false;
	}
}