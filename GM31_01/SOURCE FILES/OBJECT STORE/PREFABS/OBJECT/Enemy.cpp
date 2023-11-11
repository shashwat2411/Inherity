#include "../OBJECT STORE/prefab.h"

void ENEMY::Start()
{
	Init();

	AddComponent<MeshFilter>()->SetModel(ModelReader::GetReadModel(ModelReader::ENEMY_M));
	AddComponent<EnemyScript>();

	AddMaterial<Unlit>();

	freezeY = true;

	sprintf(tag, "enemy");

	transform->Scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
	transform->Position = D3DXVECTOR3(-4.0f, 1.0f, 0.0f);
}