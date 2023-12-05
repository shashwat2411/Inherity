#include "component.h"
#include "manager.h"

void BoxCollider::Start()
{
	multiple = true;
	isTrigger = true;
	isKinematic = false;

	CollisionSize = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	scaleOffset = 1.0f;

	//----------------------------------------------------------------
	collider = Manager::GetScene()->AddGameObject<BOXCOLLIDER>("", COLLIDER_LAYER);
	collider->Parent = gameObject;

	std::string name = collider->Parent->GetTag();
	collider->SetTag(name);
	Manager::GetScene()->SetGameObjectName(collider, COLLIDER_LAYER);

	scaleOffset = 1.0f / gameObject->transform->Scale.x;

	collider->transform->Scale = D3DXVECTOR3(CollisionSize.x, CollisionSize.y, CollisionSize.z) * scaleOffset;


#ifdef DEBUG
	collider->GetComponent<MeshFilter>()->SetEnabled(true);
#endif
}

void BoxCollider::End()
{

}

void BoxCollider::Update()
{

}

void BoxCollider::Draw()
{
	collider->transform->Scale = D3DXVECTOR3(CollisionSize.x, CollisionSize.y, CollisionSize.z) * scaleOffset;
}

void BoxCollider::EngineDisplay()
{
	if (ImGui::TreeNode("Box Collider"))
	{
		//char str[22];

		DebugManager::BoolDisplay(&isTrigger, -200.0f, "IsTrigger", 0);
		DebugManager::BoolDisplay(&isKinematic, -146.0f, "Kinematic", 1);

		ImGui::Text("\nCollision Size");
		ImGui::PushID(2);
		ImGui::DragFloat3("", CollisionSize, 0.01F);
		ImGui::PopID();


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

void BoxCollider::SetBone(MeshFilter* model, const char* bone)
{
	if (model->GetModel() != nullptr)
	{
		collider->transform->boneMatrix = model->GetModel()->GetBoneMatrix(bone);
	}
}