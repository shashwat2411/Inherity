#include "script.h"

void TopDownCamera::Update()
{
	gameObject->transform->Position = camera->Target->transform->Position + D3DXVECTOR3(0.0f, 5.0, -10.0f);
}

void TopDownCamera::EngineDisplay()
{
	if (ImGui::TreeNode("Top Down Camera"))
	{
		ImGui::TreePop();
		ImGui::Spacing();
	}
}