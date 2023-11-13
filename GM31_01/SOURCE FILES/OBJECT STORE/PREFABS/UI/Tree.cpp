#include "../OBJECT STORE/prefab.h"

void TREE::Start()
{
	Init();

	AddMaterial<DissolveMaterial>();

	depthShadow = true;

	GetMaterial()->SetTexture("_Texture", TextureReader::GetReadTexture(TextureReader::TREE_T));

	billboard->SetAnimate(false);
	billboard->SetATC(true);

	billboard->SetSize(D3DXVECTOR2(20.0f, 20.0f));
	billboard->SetOffset(Billboard::Y);

}