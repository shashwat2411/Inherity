#include "prefab.h"

void ENEMY::Start()
{
	Init();

	transform->Scale = D3DXVECTOR3(100.6f, 100.6f, 100.6f);
	transform->Position = D3DXVECTOR3(-4.0f, 1.0f, 0.0f);

	freezeY = true;
	reflection = true;

	SetTag("Enemy");

	AddComponent<MeshFilter>()->SetModel(ModelReader::GetReadModel(ModelReader::ENEMY_M));
	AddComponent<EnemyScript>();

	AddMaterial<UnlitMaterial>();

}