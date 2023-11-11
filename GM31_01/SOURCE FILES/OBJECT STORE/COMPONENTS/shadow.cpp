#include "../OBJECT STORE/component.h"
#include "../OBJECT STORE/baseobjects.h"
#include "../manager.h"
#include "../MANAGEMENT FUNCTIONS/scene.h"

void Shadow::Start()
{
	shadowHeight = 0.01f;

	shadow = Manager::GetScene()->AddGameObject<SHADOW>(SHADOW_LAYER);
}

void Shadow::Update()
{
	shadow->SetActive(gameObject->GetActive());
	shadow->transform->Position.x = gameObject->transform->Position.x;
	shadow->transform->Position.y = shadowHeight;
	shadow->transform->Position.z = gameObject->transform->Position.z;

	//for (auto plane : Manager::GetScene()->FindGameObjects<PLANE>())
	//{
	//	if (shadow->transform->Position.x < plane->transform->Position.x + plane->transform->Scale.x + 0.5f && shadow->transform->Position.x > plane->transform->Position.x - plane->transform->Scale.x - 0.5f)
	//	{
	//		if (shadow->transform->Position.z < plane->transform->Position.z + plane->transform->Scale.z + 0.5f && shadow->transform->Position.z > plane->transform->Position.z - plane->transform->Scale.z - 0.5f)
	//		{
	//			shadow->transform->Position.y = plane->transform->Position.y + 0.001f;
	//		}
	//	}
	//}

	//for (auto box : Manager::GetScene()->FindGameObjects<CUBE>())
	//{
	//	D3DXVECTOR3 shadowPosition = shadow->transform->Position;
	//	D3DXVECTOR3 shadowScale = shadow->transform->Scale;
	//	D3DXVECTOR3 boxPosition = box->transform->Position;
	//	D3DXVECTOR3 boxScale = box->transform->Scale;

	//	if (shadowPosition.x < boxPosition.x + boxScale.x + 0.5f && shadowPosition.x > boxPosition.x - boxScale.x - 0.5f)
	//	{
	//		if (shadowPosition.z< boxPosition.z + boxScale.z + 0.5f && shadowPosition.z > boxPosition.z - boxScale.z - 0.5f)
	//		{
	//			shadow->transform->Position.y = box->transform->Position.y + box->transform->Scale.y * 2.0f + 0.01f;
	//			return;
	//		}
	//	}
	//}

	//for (auto cylinder : Manager::GetScene()->FindGameObjects<CYLINDER>())
	//{
	//	if (shadow->DistanceFromWithY0(cylinder) < cylinder->transform->Scale.x + shadow->transform->Scale.x)
	//	{
	//		shadow->transform->Position.y = cylinder->transform->Position.y + cylinder->transform->Scale.y;
	//		return;
	//	}
	//}
}