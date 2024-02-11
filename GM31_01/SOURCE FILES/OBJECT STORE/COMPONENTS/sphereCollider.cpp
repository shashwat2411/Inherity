#include "component.h"
#include "manager.h"

#define COLLIDER_MODEL_OFFSET 1.0f

void SphereCollider::Start()
{
	multiple = true;
	isTrigger = false;
	isKinematic = false;
	collision = false;

	CollisionSize = 1.0f;
	collisionCounter = 0.0f;
	scaleOffset = 1.0f;

	//----------------------------------------------------------------
	ClearCollisionLayer();
	AddCollisionLayer(COLLISION_LAYER::PLAYER_CL);

	colliderObject = Manager::GetScene()->AddGameObject<SPHERECOLLIDER>("", COLLIDER_LAYER);
	colliderObject->Parent = gameObject;

	std::string name = colliderObject->Parent->GetTag().c_str();
	colliderObject->SetTag(name);
	Manager::GetScene()->SetGameObjectName(colliderObject, COLLIDER_LAYER);

	scaleOffset = 1.0f / gameObject->transform->Scale.x;

	float size = CollisionSize * COLLIDER_MODEL_OFFSET;
	colliderObject->transform->Scale = D3DXVECTOR3(size, size, size) * scaleOffset;

#ifdef DEBUG
	colliderObject->GetComponent<MeshFilter>()->SetEnabled(true);
#endif

}

void SphereCollider::End()
{
	colliderObject->Destroy();
}

void SphereCollider::Update()
{

}

void SphereCollider::Draw()
{
	float size = CollisionSize * COLLIDER_MODEL_OFFSET;
	colliderObject->transform->Scale = D3DXVECTOR3(size, size, size) * scaleOffset;
}

void SphereCollider::EngineDisplay()
{
	if (ImGui::TreeNode("Sphere Collider"))
	{
		//char str[22];


		DebugManager::BoolDisplay(&isTrigger, -180.0f, "IsTrigger");
		ImGui::SameLine();
		DebugManager::BoolDisplay(&isKinematic, -70.0f, "Kinematic", 1);


		DebugManager::FloatDisplay(&CollisionSize, -FLT_MIN, "Collision Size", true, D3DXVECTOR2(0.01f, 0.0f), 2);
		//ImGui::DragFloat("Collision Size", &CollisionSize, 0.01F);

		if (ImGui::TreeNode("Details"))
		{
			std::string parent = "GameObject : " + colliderObject->Parent->GetTag() + "\n";
			ImGui::Text(parent.c_str());

			ImGui::Text("Offset");
			colliderObject->transform->EngineDisplay();

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void SphereCollider::SetBone(MeshFilter* model, const char* bone)
{
	if (model->GetModel() != nullptr)
	{
		colliderObject->transform->boneMatrix = model->GetModel()->GetBoneMatrix(bone);
	}
}

void SphereCollider::AddCollisionLayer(COLLISION_LAYER value)
{
	for (COLLISION_LAYER cl : collisionLayer)
	{
		if (cl == value)
		{
			return;
		}
	}
	collisionLayer.push_back(value);
}