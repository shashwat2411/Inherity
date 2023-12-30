#include "component.h"
#include "manager.h"

void AudioListener::Start()
{
	startArea = 1.0f;
	endArea = 5.0f;

	scaleOffset = 1.0f;

	area1 = Manager::GetScene()->AddGameObject<SOUNDAREA>("", GIZMO_LAYER);
	area2 = Manager::GetScene()->AddGameObject<SOUNDAREA>("", GIZMO_LAYER);
	area1->Parent = gameObject;
	area2->Parent = gameObject;

	std::string str1 = "Lis Start - " + area1->Parent->GetTag();
	std::string str2 = "Lis End - " + area2->Parent->GetTag();
	area1->SetTag(str1);
	area2->SetTag(str2);

	Manager::GetScene()->SetGameObjectName(area1, GIZMO_LAYER);
	Manager::GetScene()->SetGameObjectName(area2, GIZMO_LAYER);

	float size1 = startArea;
	float size2 = endArea + startArea;
	area1->transform->Scale = D3DXVECTOR3(size1, size1, size1) * scaleOffset;
	area2->transform->Scale = D3DXVECTOR3(size2, size2, size2) * scaleOffset;

#ifdef DEBUG
	area1->GetComponent<MeshFilter>()->SetEnabled(true);
	area2->GetComponent<MeshFilter>()->SetEnabled(true);
#endif

	for (auto sources : Manager::GetScene()->FindGameObjectsOfType<AudioSource>())
	{
		sources->CollectListeners(this);
		int a = 0;
	}
}

void AudioListener::End()
{

}

void AudioListener::Update()
{

}

void AudioListener::Draw()
{
	float size1 = startArea;
	float size2 = endArea + startArea;
	area1->transform->Scale = D3DXVECTOR3(size1, size1, size1) * scaleOffset;
	area2->transform->Scale = D3DXVECTOR3(size2, size2, size2) * scaleOffset;
}

void AudioListener::EngineDisplay()
{
	if (ImGui::TreeNode("Audio Listener"))
	{
		DebugManager::FloatDisplay(&startArea, -FLT_MIN, "Start Area", false, D3DXVECTOR2(1.0f, 5.0f));
		DebugManager::FloatDisplay(&endArea, -FLT_MIN, "End Area", false, D3DXVECTOR2(5.0f, 25.0f), 1);

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