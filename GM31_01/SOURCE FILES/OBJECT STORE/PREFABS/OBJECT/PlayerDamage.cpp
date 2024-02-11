#include "prefab.h"
#include "manager.h"

void PLAYERDAMAGE::Start()
{
	Init();

	depthShadow = false;
	active = false;
	save = false;

	particleSystem->size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	particleSystem->SetParticleCount(25);
	particleSystem->burst = true;
	particleSystem->loop = false;
	particleSystem->SetPlay(false);

	particleSystem->life = 2.0f;

	particleSystem->SetTexture(TextureReader::EFFECT_T);
	particleSystem->GetParticle()->SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

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