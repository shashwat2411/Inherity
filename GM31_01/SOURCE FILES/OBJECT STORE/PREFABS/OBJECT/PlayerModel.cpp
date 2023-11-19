#include "prefab.h"
#include "input.h"

//BoxCollider* bcollider;

void PLAYERMODEL::Start()
{
	Init();

	model = AddComponent<MeshFilter>();
	//AddMaterial<DissolveMaterial>();
	//AddMaterial<UnlitMaterial>();

	sprintf(tag, "model");

	model->SetModel(ModelReader::GetReadModel(ModelReader::THE_BOSS_M));

	model->GetModel()->LoadAnimation(ModelReader::GetAnimation(), "Run");
	model->GetModel()->LoadAnimation(ModelReader::GetAnimation(), "Idle");
	model->GetModel()->LoadAnimation(ModelReader::GetAnimation(), "Dance");
	model->GetModel()->LoadAnimation(ModelReader::GetAnimation(), "Jump");

	model->SetDefaultAnimation("Idle");

	transform->Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	collider = AddComponent<SphereCollider>();

	//collider->collider->Parent = nullptr;
	collider->collider->parentMatrixEnable = true;
	collider->collider->parentMatrix = Mathf::MatrixConvert(model->GetModel()->GetBone()["mixamorig:LeftHand"].worldMatrix);
	collider->SetIsTrigger(true);
	collider->SetIsKinematic(true);
	collider->SetCollisionSize(0.3f);
	collider->scaleOffset = 100.0f;
	//collider->collider->transform->Position = D3DXVECTOR3(-89.0f, 144.0f, 6.0f);

	//bcollider = AddComponent<BoxCollider>();
	//bcollider->collider->parentMatrixEnable = true;
	//bcollider->collider->parentMatrix = Mathf::MatrixConvert(model->GetModel()->GetBone()["mixamorig:LeftHand"].worldMatrix);
	//bcollider->SetIsTrigger(true);
	//bcollider->SetIsKinematic(true);
	//bcollider->SetCollisionSize(D3DXVECTOR3(1.3f, 0.3f, 0.3f));
	//bcollider->scaleOffset = 100.0f;

	Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void PLAYERMODEL::Update()
{
	collider->collider->parentMatrix = Mathf::MatrixConvert(model->GetModel()->GetBone()["mixamorig:LeftHand"].worldMatrix);


	//bcollider->collider->parentMatrix = Mathf::MatrixConvert(model->GetModel()->GetBone()["mixamorig:LeftHand"].worldMatrix);

#ifdef DEBUG
	//D3DXVECTOR3 pos = collider->collider->transform->Position;
	//
	//char* str = GetDebugStr();
	//sprintf(&str[strlen(str)], " | COLLIDER | Position [ X : %.2f, Y : %0.2f, Z : %0.2f ]", pos.x, pos.y, pos.z);
#endif
}