#include "manager.h"
#include "../saveFunctions.h"

//BoxCollider* bcollider;

void PLAYERMODEL::Start()
{
	Init();

	quaternion = true;

	model = AddComponent<MeshFilter>();
	//AddMaterial<DissolveMaterial>();
	//AddMaterial<UnlitMaterial>();
	AddMaterial<ToonMaterial>();

	model->SetModel(ModelReader::TOMATO_M);

	model->GetModel()->LoadAnimation("Run");
	model->GetModel()->LoadAnimation("Idle");
	model->GetModel()->LoadAnimation("Dance");
	model->GetModel()->LoadAnimation("Jump");
	model->GetModel()->LoadAnimation("Roll");

	model->GetModel()->LoadAnimation("Punch_Left");
	model->GetModel()->LoadAnimation("Punch_Right");
	model->GetModel()->LoadAnimation("Low_Punch_Left");
	model->GetModel()->LoadAnimation("Low_Punch_Right");
	model->GetModel()->LoadAnimation("Cross_Punch_Right");

	model->GetModel()->LoadAnimation("Kick_1");
	model->GetModel()->LoadAnimation("Kick_2");
	model->GetModel()->LoadAnimation("Kick_3");
	model->GetModel()->LoadAnimation("Kick_4");

	model->SetDefaultAnimation("Idle");

	transform->Scale = D3DXVECTOR3(0.007f, 0.007f, 0.007f);
	transform->Position.y = -1.0f;

	collider = AddComponent<SphereCollider>();
	collider->SetBone(model, "mixamorig:LeftHand");
	collider->SetIsTrigger(false);
	collider->SetIsKinematic(false);
	collider->SetCollisionSize(0.3f);

	collider = AddComponent<SphereCollider>();
	collider->SetBone(model, "mixamorig:RightHand");
	collider->SetIsTrigger(false);
	collider->SetIsKinematic(false);
	collider->SetCollisionSize(0.3f);

	collider = AddComponent<SphereCollider>();
	collider->SetBone(model, "mixamorig:RightFoot");
	collider->SetIsTrigger(false);
	collider->SetIsKinematic(false);
	collider->SetCollisionSize(0.3f);

	collider = AddComponent<SphereCollider>();
	collider->SetBone(model, "mixamorig:LeftFoot");
	collider->SetIsTrigger(false);
	collider->SetIsKinematic(false);
	collider->SetCollisionSize(0.3f);

	GAMEOBJECT* damager = Manager::GetScene()->AddGameObject<EMPTYOBJECT>("Damager");
	damager->SetParent(this);

	collider = damager->AddComponent<SphereCollider>();
	collider->SetBone(model, "mixamorig:Spine");
	collider->SetIsTrigger(false);
	collider->SetIsKinematic(false);
	collider->SetCollisionSize(1.44f);
	collider->scaleOffset = 100.0f;

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