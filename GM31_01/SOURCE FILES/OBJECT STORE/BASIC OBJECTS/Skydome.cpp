#include "baseobjects.h"
#include "manager.h"

void SKYDOME::Init()
{
	Initialize();

	quaternion = true;

	transform->culling = false;

	transform->Position.y = -10.0f;
	transform->Scale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	AddComponent<MeshFilter>()->SetModel(ModelReader::SKYDOME_M);

	AddMaterial<UnlitMaterial>();
	SetDepthShadow(false);
	SetReflection(true);
}

void SKYDOME::Update()
{
	SCENE* scene = Manager::GetScene();
	if (scene->GetPlayer() != nullptr)
	{
		D3DXVECTOR3 pos = scene->GetPlayer()->transform->Position;
		transform->Position.x = pos.x;
		transform->Position.z = pos.z;

		D3DXQUATERNION quat;
		D3DXQuaternionRotationAxis(&quat, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.001f);
		transform->Quaternion *= quat;
	}
}