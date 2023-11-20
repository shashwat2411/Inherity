#include "component.h"
#include "manager.h"

void AudioListener::Start()
{
	startArea = 1.0f;
	endArea = 5.0f;

	scaleOffset = 1.0f;

	area1 = Manager::GetScene()->AddGameObject<SOUNDAREA>(GIZMO_LAYER);
	area2 = Manager::GetScene()->AddGameObject<SOUNDAREA>(GIZMO_LAYER);
	area1->Parent = gameObject;
	area2->Parent = gameObject;

	float size1 = startArea;
	float size2 = endArea + startArea;
	area1->transform->Scale = D3DXVECTOR3(size1, size1, size1) * scaleOffset;
	area2->transform->Scale = D3DXVECTOR3(size2, size2, size2) * scaleOffset;

#ifdef DEBUG
	area1->GetComponent<MeshFilter>()->SetEnabled(true);
	area2->GetComponent<MeshFilter>()->SetEnabled(true);
#endif
}

void AudioListener::End()
{

}

void AudioListener::Update()
{
	float size1 = startArea;
	float size2 = endArea + startArea;
	area1->transform->Scale = D3DXVECTOR3(size1, size1, size1) * scaleOffset;
	area2->transform->Scale = D3DXVECTOR3(size2, size2, size2) * scaleOffset;
}

void AudioListener::Draw()
{

}