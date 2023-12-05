#include "prefab.h"
#include "animations.h"

void ENEMY::Start()
{
	Init();

	transform->Scale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);
	transform->Position = D3DXVECTOR3(-4.0f, 1.0f, 0.0f);

	freezeY = true;
	reflection = true;

	AddComponent<MeshFilter>()->SetModel(ModelReader::ENEMY_M);

	AddComponent<EnemyScript>();
	
	AddComponent<Animator>()->AddAnimation<TrialAnimation>();

	AddMaterial<UnlitMaterial>();

}