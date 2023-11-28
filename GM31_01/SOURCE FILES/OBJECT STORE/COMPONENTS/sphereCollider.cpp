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
	colliderObject = Manager::GetScene()->AddGameObject<SPHERECOLLIDER>(COLLIDER_LAYER);
	colliderObject->Parent = gameObject;
	colliderObject->SetTag(colliderObject->Parent->GetTag());

	float size = CollisionSize * COLLIDER_MODEL_OFFSET;
	colliderObject->transform->Scale = D3DXVECTOR3(size, size, size) * scaleOffset;

#ifdef DEBUG
	colliderObject->GetComponent<MeshFilter>()->SetEnabled(true);
#endif

}

void SphereCollider::End()
{

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
			std::string parent = "Parent : " + colliderObject->Parent->GetTag() + "\n";
			ImGui::Text(parent.c_str());

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