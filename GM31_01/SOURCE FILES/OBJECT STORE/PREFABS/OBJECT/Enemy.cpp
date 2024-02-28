#include "prefab.h"
#include "animations.h"
#include "manager.h"

void ENEMY::Start()
{
	Init();

	transform->Scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
	transform->Position = D3DXVECTOR3(-109.88f, 0.0f, 32.8f);

	freezeY = true;
	reflection = true;

	EMPTYOBJECT* child = Manager::GetScene()->AddGameObject<EMPTYOBJECT>(ObjectIndex("Enemy Model"));
	child->SetParent(this);
	child->AddComponent<MeshFilter>()->SetModel(ModelReader::HUMAN_M);
	child->transform->Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	MeshFilter* model = child->GetComponent<MeshFilter>();
	model->GetModel()->LoadAnimation("Enemy_Walk");
	model->GetModel()->LoadAnimation("Enemy_Find");
	model->GetModel()->LoadAnimation("Enemy_Attack_1");
	model->GetModel()->LoadAnimation("Enemy_Run");
	model->GetModel()->LoadAnimation("Enemy_Damage");
	model->GetModel()->LoadAnimation("Enemy_Wait");
	model->GetModel()->LoadAnimation("Enemy_Death");
	model->GetModel()->LoadAnimation("Enemy_Dance_1");
	model->GetModel()->LoadAnimation("Enemy_Dance_2");
	model->GetModel()->LoadAnimation("Enemy_Pose_1");
	model->GetModel()->LoadAnimation("Enemy_Pose_2");

	model->SetDefaultAnimation("Enemy_Walk");

	child->AddMaterial<ToonMaterial>();

	AddComponent<EnemyScript>();
	AddComponent<ArtificialIntelligence>();


	EMPTYOBJECT* knife = Manager::GetScene()->AddGameObject<EMPTYOBJECT>(ObjectIndex("Knife"));
	knife->SetParent(child);
	knife->AddComponent<MeshFilter>()->SetModel(ModelReader::KNIFE_M);

	knife->transform->boneMatrix = model->GetModel()->GetBoneMatrix("mixamorig:RightHand");
	knife->transform->Position.y = 18.9f;
	knife->transform->Rotation = D3DXVECTOR3(127.0f, -28.5f, -69.5f);
	knife->transform->Scale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

	knife->AddMaterial<ToonPhongMaterial>()->SetFloat("_Threshold", 1.0f);

	SphereCollider* knifeCollider = knife->AddComponent<SphereCollider>();
	knifeCollider->scaleOffset = 1.0f;
	knifeCollider->GetColliderObject()->transform->Position.y = 1.8f;
	knifeCollider->SetCollisionSize(0.26f);
	knifeCollider->ClearCollisionLayer();
	knifeCollider->SetSelfLayer(COLLISION_LAYER::KNIFE_CL);
	knifeCollider->AddCollisionLayer(COLLISION_LAYER::PLAYER_CL);

	knife->AddComponent<KnifeCollision>();

	AddComponent<EnemyHealth>();

}