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
	collider = Manager::GetScene()->AddGameObject<BOXCOLLIDER>(COLLIDER_LAYER);
	collider->Parent = gameObject;

	collider->transform->Scale = D3DXVECTOR3(CollisionSize.x, CollisionSize.y, CollisionSize.z) * scaleOffset;

	//Front
	vertex[0] = D3DXVECTOR3( CollisionSize.x / 2.0f,  CollisionSize.y / 2.0f,  CollisionSize.z / 2.0f);
	vertex[1] = D3DXVECTOR3(-CollisionSize.x / 2.0f,  CollisionSize.y / 2.0f,  CollisionSize.z / 2.0f);
	vertex[2] = D3DXVECTOR3( CollisionSize.x / 2.0f, -CollisionSize.y / 2.0f,  CollisionSize.z / 2.0f);
	vertex[3] = D3DXVECTOR3(-CollisionSize.x / 2.0f, -CollisionSize.y / 2.0f,  CollisionSize.z / 2.0f);

	//Back
	vertex[4] = D3DXVECTOR3( CollisionSize.x / 2.0f,  CollisionSize.y / 2.0f, -CollisionSize.z / 2.0f);
	vertex[5] = D3DXVECTOR3(-CollisionSize.x / 2.0f,  CollisionSize.y / 2.0f, -CollisionSize.z / 2.0f);
	vertex[6] = D3DXVECTOR3( CollisionSize.x / 2.0f, -CollisionSize.y / 2.0f, -CollisionSize.z / 2.0f);
	vertex[7] = D3DXVECTOR3(-CollisionSize.x / 2.0f, -CollisionSize.y / 2.0f, -CollisionSize.z / 2.0f);

	for (int i = 0; i < 8; i++) { vertex[i] += collider->transform->GlobalPosition; }

#ifdef DEBUG
	collider->GetComponent<MeshFilter>()->SetEnabled(true);
#endif
}

void BoxCollider::End()
{

}

void BoxCollider::Update()
{
	//Front
	vertex[0] = D3DXVECTOR3( CollisionSize.x / 2.0f,  CollisionSize.y / 2.0f,  CollisionSize.z / 2.0f);
	vertex[1] = D3DXVECTOR3(-CollisionSize.x / 2.0f,  CollisionSize.y / 2.0f,  CollisionSize.z / 2.0f);
	vertex[2] = D3DXVECTOR3( CollisionSize.x / 2.0f, -CollisionSize.y / 2.0f,  CollisionSize.z / 2.0f);
	vertex[3] = D3DXVECTOR3(-CollisionSize.x / 2.0f, -CollisionSize.y / 2.0f,  CollisionSize.z / 2.0f);

	//Back
	vertex[4] = D3DXVECTOR3( CollisionSize.x / 2.0f,  CollisionSize.y / 2.0f, -CollisionSize.z / 2.0f);
	vertex[5] = D3DXVECTOR3(-CollisionSize.x / 2.0f,  CollisionSize.y / 2.0f, -CollisionSize.z / 2.0f);
	vertex[6] = D3DXVECTOR3( CollisionSize.x / 2.0f, -CollisionSize.y / 2.0f, -CollisionSize.z / 2.0f);
	vertex[7] = D3DXVECTOR3(-CollisionSize.x / 2.0f, -CollisionSize.y / 2.0f, -CollisionSize.z / 2.0f);

	for (int i = 0; i < 8; i++) { vertex[i] += collider->transform->GlobalPosition; }

	collider->transform->Scale = D3DXVECTOR3(CollisionSize.x, CollisionSize.y, CollisionSize.z) * scaleOffset;
}

void BoxCollider::Draw()
{

}

void BoxCollider::EngineDisplay()
{
	if (ImGui::TreeNode("Box Collider"))
	{
		//char str[22];

		DebugManager::BoolDisplay(&isTrigger, -200.0f, "IsTrigger");
		DebugManager::BoolDisplay(&isKinematic, -146.0f, "Kinematic", 1);

		ImGui::Text("\nCollision Size");
		ImGui::DragFloat3("", CollisionSize, 0.01F);

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