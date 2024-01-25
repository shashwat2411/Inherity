#include "customScenes.h"

void GEOMETRY_INSTANCING_SCENE::Init()
{
	name = "Geometry Instancing";

	//GAMEOBJECT
	PLANE* Field = AddGameObject<PLANE>("Field");
	//BILLBOARD* tree = AddGameObject<TREE>("Tree(Clone)", BILLBOARD_LAYER);
	//tree->AddMaterial<GeometryInstancingMaterial>()->SetTexture("_Texture", TextureReader::TREE_T);

	//UI

	//eŽqŠÖŒWÝ’è
	{

	}

	//Ý’è
	{
		Field->GetMaterial()->SetTexture("_Texture", TextureReader::GROUND_T);
		Field->meshField->TexCoord = D3DXVECTOR2(10.0f, 10.0f);
		Field->meshField->Size = D3DXVECTOR2(5.0f, 5.0f);
		Field->transform->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Field->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//Field->transform->Rotation = D3DXVECTOR3(0.84f, 0.0f, 0.0f);
		Field->meshField->RecreateField();
	}

	//‰¹
	{
		//SoundReader::GetReadSound(SoundReader::GAME)->Play(true, 0.2f);
	}
}

void GEOMETRY_INSTANCING_SCENE::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN)) { end = true; }
	if (end == true && Fade->GetFadeIn() == false) { if (Fade->FadeOut() == false) { Manager::SetScene<TITLE_SCENE>(); } }
}