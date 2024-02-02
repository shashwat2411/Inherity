#include "prefab.h"
#include "manager.h"

void MAPCOLLISIONOBJECT::Start()
{
	Init();

	depthShadow = false;

	Color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);

	AddComponent<MeshFilter>()->SetModel(ModelReader::CUBE_M);
	AddComponent<MapCollision>();
}