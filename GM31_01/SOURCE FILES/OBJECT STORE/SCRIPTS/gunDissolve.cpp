#include "script.h"
#include "manager.h"

void GunDissolve::Start()
{
	timerVector["Threshold"] = 0.0f;
	timerVector["Dissolve_Range"] = 0.04f;
	dissolveColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	//gameObject->GetMaterial()->SetTexture("_Texture", TextureReader::WIPE_T);
}

void GunDissolve::End()
{

}

void GunDissolve::Update()
{

}

void GunDissolve::Draw()
{
	timerVector["Threshold"] = gameObject->GetMaterial()->GetFloat("_Threshold");
	gameObject->GetMaterial()->SetFloat("_Dissolve_Range", timerVector["Dissolve_Range"]);
	dissolveColor = gameObject->GetMaterial()->GetColor("_Dissolve_Color");
}

void GunDissolve::EngineDisplay()
{
	if (ImGui::TreeNode("Gun Dissolve"))
	{
		DebugManager::FloatDisplay(&timerVector["Threshold"], -FLT_MIN, "Threshold", true, D3DXVECTOR2(0.01f, 0.0f), 0, true);
		DebugManager::FloatDisplay(&timerVector["Dissolve_Range"], -FLT_MIN, "Dissolve_Range", true, D3DXVECTOR2(0.01f, 0.0f), 2);

		ImGui::PushID(3);
		ImGui::Text("\n");
		ImGui::ColorPicker4("Dissolve Color\n", dissolveColor);
		ImGui::PopID();

		ImGui::Text("Texture : ");
		ImGui::SameLine();

		ImGui::PushID(4);
		if (ImGui::Combo("", gameObject->GetMaterial()->GetIndex(), TextureReader::GetTextureNames(), TextureReader::READ_TEXTURE_MAX))
		{
			gameObject->GetMaterial()->SetTexture("_Texture", ((TextureReader::READ_TEXTURE)*gameObject->GetMaterial()->GetIndex()));
		}
		ImGui::PopID();

		ImGui::TreePop();
		ImGui::Spacing();
	}
}