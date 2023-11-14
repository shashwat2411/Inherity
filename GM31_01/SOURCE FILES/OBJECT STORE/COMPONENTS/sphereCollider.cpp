#include "component.h"
#include "manager.h"

#define COLLIDER_MODEL_OFFSET 1.0f

void SphereCollider::Start()
{
	multiple = true;
	isTrigger = false;
	isKinematic = false;

	CollisionSize = 1.0f;
	scaleOffset = 1.0f;

	//----------------------------------------------------------------
	collider = Manager::GetScene()->AddGameObject<SPHERECOLLIDER>(COLLIDER_LAYER);
	collider->Parent = gameObject;

	float size = CollisionSize * COLLIDER_MODEL_OFFSET;
	collider->transform->Scale = D3DXVECTOR3(size, size, size) * scaleOffset;

#ifdef DEBUG
	collider->GetComponent<MeshFilter>()->SetEnabled(true);
#endif

}

void SphereCollider::End()
{

}

void SphereCollider::Update()
{
	float size = CollisionSize * COLLIDER_MODEL_OFFSET;
	collider->transform->Scale = D3DXVECTOR3(size, size, size) * scaleOffset;

#ifdef DEBUG
	//char* str = GetDebugStr();
	//sprintf(&str[strlen(str)], " | Collider Position [ X : %.2f, Y : %.2f, Z : %.2f ]", collider->transform->GlobalPosition.x, collider->transform->GlobalPosition.y, collider->transform->GlobalPosition.z);
#endif
}

void SphereCollider::Draw()
{

}
