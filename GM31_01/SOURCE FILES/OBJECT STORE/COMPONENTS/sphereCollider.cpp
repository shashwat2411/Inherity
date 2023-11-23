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
	collider->SetTag(collider->Parent->GetTag());

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
			std::string parent = "Parent : " + collider->Parent->GetTag() + "\n";
			ImGui::Text(parent.c_str());

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}
}
