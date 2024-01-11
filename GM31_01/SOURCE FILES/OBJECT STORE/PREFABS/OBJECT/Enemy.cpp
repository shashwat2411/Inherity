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
	child->transform->Scale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);

	MeshFilter* model = child->GetComponent<MeshFilter>();
	model->GetModel()->LoadAnimation("Enemy_Walk");

	model->SetDefaultAnimation("Enemy_Walk");

	AddComponent<EnemyScript>();
	AddComponent<ArtificialIntelligence>();

	AddMaterial<UnlitMaterial>();
}