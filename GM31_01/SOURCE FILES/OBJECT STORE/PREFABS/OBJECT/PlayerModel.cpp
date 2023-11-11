#include "../OBJECT STORE/prefab.h"

void PLAYERMODEL::Start()
{
	Init();

	model = AddComponent<MeshFilter>();
	//AddMaterial<Dissolve>();
	//AddMaterial<Unlit>();

	sprintf(tag, "model");

	model->SetModel(ModelReader::GetReadModel(ModelReader::THE_BOSS_M));

	model->GetModel()->LoadAnimation(ModelReader::GetAnimation(), "Run");
	model->GetModel()->LoadAnimation(ModelReader::GetAnimation(), "Idle");
	model->GetModel()->LoadAnimation(ModelReader::GetAnimation(), "Dance");
	model->GetModel()->LoadAnimation(ModelReader::GetAnimation(), "Jump");

	model->SetDefaultAnimation("Idle");

	transform->Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	collider = AddComponent<SphereCollider>();
	collider->collider->SetDepthShadow(true);

	//collider->collider->Parent = nullptr;
	collider->collider->transform->Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	collider->collider->parentMatrixEnable = true;
	collider->collider->parentMatrix = Mathf::MatrixConvert(model->GetModel()->GetBone()["mixamorig:LeftHand"].Matrix);
	collider->SetIsTrigger(true);
	collider->SetCollisionSize(0.2f);

	Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void PLAYERMODEL::Update()
{
	collider->collider->parentMatrix = Mathf::MatrixConvert(model->GetModel()->GetBone()["mixamorig:LeftHand"].Matrix);

#ifdef DEBUG
	D3DXVECTOR3 pos = collider->collider->transform->GlobalPosition;

	char* str = GetDebugStr();
	sprintf(&str[strlen(str)], " | COLLIDER | Position [ X : %.2f, Y : %0.2f, Z : %0.2f ]", pos.x, pos.y, pos.z);
#endif
}