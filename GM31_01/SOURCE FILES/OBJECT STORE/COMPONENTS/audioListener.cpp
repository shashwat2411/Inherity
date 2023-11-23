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

	std::string str1 = "Lis Start - " + area1->Parent->GetTag();
	std::string str2 = "Lis End - " + area2->Parent->GetTag();
	area1->SetTag(str1);
	area2->SetTag(str2);

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

void AudioListener::EngineDisplay()
{
	if (ImGui::TreeNode("Audio Listener"))
	{
		char str[22];

		ImGui::PushItemWidth(-FLT_MIN);
		sprintf_s(str, sizeof(str), "Start Area : %.2f", startArea);
		ImGui::SliderFloat(" ", &startArea, 1.0f, 5.0f, str);

		ImGui::PushItemWidth(-FLT_MIN);
		sprintf_s(str, sizeof(str), "End Area : %.2f", endArea);
		ImGui::SliderFloat("  ", &endArea, 5.0f, 25.0f, str);

		if (ImGui::TreeNode("Details"))
		{
			std::string parent = "Parent : " + area1->Parent->GetTag() + "\n";
			ImGui::Text(parent.c_str());

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}
}