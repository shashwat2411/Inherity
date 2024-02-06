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
	model->GetModel()->LoadAnimation("Death");
	model->GetModel()->LoadAnimation("Anger");

	model->GetModel()->LoadAnimation("Forward_Jog");
	model->GetModel()->LoadAnimation("Backward_Jog");
	model->GetModel()->LoadAnimation("Left_Jog");
	model->GetModel()->LoadAnimation("Right_Jog");

	model->GetModel()->LoadAnimation("Forward_Left_Jog");
	model->GetModel()->LoadAnimation("Forward_Right_Jog");
	model->GetModel()->LoadAnimation("Backward_Left_Jog");
	model->GetModel()->LoadAnimation("Backward_Right_Jog");

	model->SetDefaultAnimation("Idle");

	transform->culling = false;
	transform->Scale = D3DXVECTOR3(0.007f, 0.007f, 0.007f);
	transform->Position.y = -1.0f;

	//collider = AddComponent<SphereCollider>();
	//collider->SetBone(model, "mixamorig:LeftHand");
	//collider->SetIsTrigger(false);
	//collider->SetIsKinematic(false);
	//collider->SetCollisionSize(0.3f);

	//collider = AddComponent<SphereCollider>();
	//collider->SetBone(model, "mixamorig:RightHand");
	//collider->SetIsTrigger(false);
	//collider->SetIsKinematic(false);
	//collider->SetCollisionSize(0.3f);

	//collider = AddComponent<SphereCollider>();
	//collider->SetBone(model, "mixamorig:RightFoot");
	//collider->SetIsTrigger(false);
	//collider->SetIsKinematic(false);
	//collider->SetCollisionSize(0.3f);

	//collider = AddComponent<SphereCollider>();
	//collider->SetBone(model, "mixamorig:LeftFoot");
	//collider->SetIsTrigger(false);
	//collider->SetIsKinematic(false);
	//collider->SetCollisionSize(0.3f);

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

	GAMEOBJECT* gun = Manager::GetScene()->AddGameObject<EMPTYOBJECT>("Gun Left", LATEOBJECT_LAYER);
	gun->AddComponent<MeshFilter>()->SetModel(ModelReader::TOMATO_GUN_M);
	gun->SetParent(model->gameObject);
	gun->transform->boneMatrix = model->GetModel()->GetBoneMatrix("mixamorig:Spine");
	gun->AddMaterial<ToonPhongMaterial>()->SetFloat("_Dissolve", 1.0f);

	GAMEOBJECT* spawnPoint = Manager::GetScene()->AddGameObject<EMPTYOBJECT>("Spawn Point Left", LATEOBJECT_LAYER);
	spawnPoint->SetParent(gun);
	spawnPoint->transform->Position = D3DXVECTOR3(2.2f, -0.35f, 0.0f);

	gun = Manager::GetScene()->AddGameObject<EMPTYOBJECT>("Gun Right", LATEOBJECT_LAYER);
	gun->AddComponent<MeshFilter>()->SetModel(ModelReader::TOMATO_GUN_M);
	gun->SetParent(model->gameObject);
	gun->transform->boneMatrix = model->GetModel()->GetBoneMatrix("mixamorig:Spine");
	gun->AddMaterial<ToonPhongMaterial>()->SetFloat("_Dissolve", 1.0f);

	spawnPoint = Manager::GetScene()->AddGameObject<EMPTYOBJECT>("Spawn Point Right", LATEOBJECT_LAYER);
	spawnPoint->SetParent(gun);
	spawnPoint->transform->Position = D3DXVECTOR3(2.2f, -0.35f, 0.0f);


	Color = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
}

void PLAYERMODEL::Update()
{

}