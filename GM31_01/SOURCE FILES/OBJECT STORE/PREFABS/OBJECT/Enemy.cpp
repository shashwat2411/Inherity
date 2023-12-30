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

	EMPTYOBJECT* model = Manager::GetScene()->AddGameObject<EMPTYOBJECT>("Enemy Model");
	model->SetParent(this);
	model->AddComponent<MeshFilter>()->SetModel(ModelReader::ENEMY_M);

	AddComponent<EnemyScript>();
	AddComponent<ArtificialIntelligence>();

	AddMaterial<UnlitMaterial>();
}