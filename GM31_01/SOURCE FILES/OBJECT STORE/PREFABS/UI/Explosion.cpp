#include "../OBJECT STORE/prefab.h"

void EXPLOSION::Start()
{
	Init();

	billboard->SetAnimate(true);
	billboard->SetElements(4, 4);

	GetMaterial()->SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::EXPLOSION_T));
}