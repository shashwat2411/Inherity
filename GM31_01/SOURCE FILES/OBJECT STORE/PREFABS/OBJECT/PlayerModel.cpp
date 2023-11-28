#include "prefab.h"
#include "input.h"

//BoxCollider* bcollider;

void PLAYERMODEL::Start()
{
	Init();

	model = AddComponent<MeshFilter>();
	//AddMaterial<DissolveMaterial>();
	//AddMaterial<UnlitMaterial>();

	SetTag("PlayerModel");

	model->SetModel(ModelReader::GetReadModel(ModelReader::THE_BOSS_M));

	model->GetModel()->LoadAnimation("Run");
	model->GetModel()->LoadAnimation("Idle");
	model->GetModel()->LoadAnimation("Dance");
	model->GetModel()->LoadAnimation("Jump");

	model->SetDefaultAnimation("Idle");

	transform->Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	collider = AddComponent<SphereCollider>();

	collider->SetBone(model, "mixamorig:LeftHand");

	collider->SetIsTrigger(false);
	collider->SetIsKinematic(false);
	collider->SetCollisionSize(0.3f);
	collider->scaleOffset = 100.0f;


	Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void PLAYERMODEL::Update()
{

}