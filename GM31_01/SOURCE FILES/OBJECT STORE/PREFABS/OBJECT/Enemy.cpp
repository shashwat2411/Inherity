#include "prefab.h"
#include "animations.h"
#include "manager.h"

void ENEMY::Start()
{
	Init();

	transform->Scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
	transform->Position = D3DXVECTOR3(-4.0f, 1.0f, 0.0f);

	freezeY = true;
	reflection = true;

	EMPTYOBJECT* child = Manager::GetScene()->AddGameObject<EMPTYOBJECT>("Enemy Model");
	child->SetParent(this);
	child->AddComponent<MeshFilter>()->SetModel(ModelReader::HUMAN_M);
	child->transform->Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	MeshFilter* model = child->GetComponent<MeshFilter>();
	model->GetModel()->LoadAnimation("Enemy_Walk");
	model->GetModel()->LoadAnimation("Enemy_Find");

	model->SetDefaultAnimation("Enemy_Walk");

	model->gameObject->AddMaterial<ToonMaterial>();

	AddComponent<EnemyScript>();
	AddComponent<ArtificialIntelligence>();


	EMPTYOBJECT* knife = Manager::GetScene()->AddGameObject<EMPTYOBJECT>("Knife");
	knife->SetParent(model->gameObject);
	knife->AddComponent<MeshFilter>()->SetModel(ModelReader::KNIFE_M);

	knife->transform->boneMatrix = model->GetModel()->GetBoneMatrix("mixamorig:RightHand");
	knife->transform->Position.y = 18.9f;
	knife->transform->Rotation = D3DXVECTOR3(127.0f, -28.5f, -69.5f);
	knife->transform->Scale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	knife->AddMaterial<ToonPhongMaterial>();
	knife->AddComponent<SphereCollider>()->scaleOffset = 1.0f;
	knife->GetComponent<SphereCollider>()->GetColliderObject()->transform->Position.y = 1.1f;

}