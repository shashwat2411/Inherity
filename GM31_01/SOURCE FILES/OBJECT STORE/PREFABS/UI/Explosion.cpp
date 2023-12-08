#include "prefab.h"

void EXPLOSION::Start()
{
	Init();

	billboard->SetAnimate(true);
	billboard->SetElements(4, 4);
	transform->Scale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);

	GetMaterial()->SetTexture("_Texture", TextureReader::EXPLOSION_T);
}