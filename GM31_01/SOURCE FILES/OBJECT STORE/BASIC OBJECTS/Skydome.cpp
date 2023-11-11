#include "../OBJECT STORE/baseobjects.h"
#include "../manager.h"

void SKYDOME::Init()
{
	Initialize();

	transform->culling = false;

	transform->Position.y = -3.0f;
	transform->Scale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	AddComponent<MeshFilter>()->SetModel(ModelReader::GetReadModel(ModelReader::SKYDOME_M));

	AddMaterial<Unlit>();
	SetDepthShadow(false);

}

void SKYDOME::Update()
{
	SCENE* scene = Manager::GetScene();
	if (scene->GetPlayer() != nullptr)
	{
		D3DXVECTOR3 pos = scene->GetPlayer()->transform->Position;
		transform->Position.x = pos.x;
		transform->Position.z = pos.z;

		transform->Rotation.y += 0.04f;
	}
}