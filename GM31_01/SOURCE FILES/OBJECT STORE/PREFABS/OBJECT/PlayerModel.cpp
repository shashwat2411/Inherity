#include "prefab.h"
#include "input.h"

//BoxCollider* bcollider;

void PLAYERMODEL::Start()
{
	Init();

	quaternion = true;

	model = AddComponent<MeshFilter>();
	//AddMaterial<DissolveMaterial>();
	//AddMaterial<UnlitMaterial>();

	model->SetModel(ModelReader::TOMATO_M);

	model->GetModel()->LoadAnimation("Run");
	model->GetModel()->LoadAnimation("Idle");
	model->GetModel()->LoadAnimation("Dance");
	model->GetModel()->LoadAnimation("Jump");

	model->GetModel()->LoadAnimation("Punch_Left");
	model->GetModel()->LoadAnimation("Punch_Right");
	model->GetModel()->LoadAnimation("Low_Punch_Left");
	model->GetModel()->LoadAnimation("Low_Punch_Right");

	model->SetDefaultAnimation("Idle");

	transform->Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	collider = AddComponent<SphereCollider>();
	collider->SetBone(model, "mixamorig:LeftHand");
	collider->SetIsTrigger(false);
	collider->SetIsKinematic(false);
	collider->SetCollisionSize(0.3f);

	//BoxCollider* bcollider = AddComponent<BoxCollider>();
	//bcollider->SetBone(model, "mixamorig:LeftHand");
	//bcollider->SetIsTrigger(false);
	//bcollider->SetIsKinematic(false);
	//bcollider->SetCollisionSize(D3DXVECTOR3(0.3f, 0.3f, 0.3f));
	//bcollider->scaleOffset = 100.0f;


	Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void PLAYERMODEL::Update()
{

}