#include "component.h"
#include "manager.h"
#include "material.h"
#include "modelReader.h"


void MeshFilter::Start()
{
	fbx = false;

	modelIndex = 0;

	multiple = true;
	loop = true;

	blendRate = 0.0f;
	blendSpeed = 0.05f;
	frame = 0.0f;
	time = 0.0f;

	m_Model = nullptr;
	m_Model_obj = nullptr;

	gameObject->AddMaterial<DefaultMaterial>();
}

void MeshFilter::End()
{
	if (m_Model != nullptr)
	{
		m_Model = nullptr;
	}

	if (m_Model_obj != nullptr)
	{
		m_Model_obj = nullptr;
	}
}

void MeshFilter::Update()
{
	if (fbx == true)
	{
		if (m_Model != nullptr)
		{
			blendRate += blendSpeed * Time::fixedTimeScale;
			if (blendRate > 1.0f) { blendRate = 1.0f; }

			int t = (int)time;
			m_Model->Update(animationName.c_str(), t, animationBlendName.c_str(), (t + 1), blendRate, time);

			frame += Time::fixedTimeScale;
			time += Time::fixedTimeScale;
		}
	}
}

void MeshFilter::Draw()
{
	if (Manager::GetScene()->GetCamera()->camera->CheckView(gameObject->transform) == true)
	{
		if (fbx == true)
		{
			if (m_Model != nullptr)
			{
				m_Model->Draw();
			}
		}
		else
		{
			if (m_Model_obj != nullptr)
			{
				m_Model_obj->Draw();
			}
		}
	}
}

void MeshFilter::EngineDisplay()
{
	if (ImGui::TreeNode("Mesh Filter"))
	{
		ImGui::Text("Model : ");
		ImGui::SameLine();

		ImGui::PushID(0);
		if (ImGui::Combo("", &modelIndex, ModelReader::GetModelNames(), (ModelReader::READ_MODEL_OBJ_MAX + ModelReader::READ_MODEL_FBX_MAX)))
		{
			if (modelIndex < ModelReader::READ_MODEL_OBJ_MAX)
			{
				SetModel((ModelReader::READ_MODEL_OBJ)modelIndex);
			}
			else
			{
				SetModel((ModelReader::READ_MODEL_FBX)(modelIndex - ModelReader::READ_MODEL_OBJ_MAX));
			}
		}
		ImGui::PopID();

		if (ImGui::TreeNode("Details"))
		{
			DebugManager::BoolDisplay(&fbx, -200.0f, "FBX", 0, true);
			if (fbx)
			{
				std::string name;

				name = "	Animation 1 : " + animationName;
				ImGui::Text(name.c_str());

				name = "	Animation 2 : " + animationBlendName;
				ImGui::Text(name.c_str());


				DebugManager::FloatDisplay(&time, -FLT_MIN, "Time", true, D3DXVECTOR2(0.01f, 0.0f), 1, true);
				//ImGui::LabelText("##Label", "	Time : %.2f", time);
				DebugManager::BoolDisplay(&loop, -200.0f, "Loop", 2, true);
			}

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

bool MeshFilter::GetAnimationOver(const char* name)
{
	if (m_Model != nullptr)
	{
		if (animationBlendName == name)
		{
			return m_Model->over;
		}
	}

	return false;
}

void MeshFilter::SetAnimationBlend(const char* name, bool lp, float speed)
{
	if (m_Model != nullptr)
	{
		if (animationBlendName != name)
		{
			loop = lp;

			time = 0.0f;
			//frame = 0.0f;
			blendRate = 0.0f;

			animationName = animationBlendName;
			animationBlendName = name;

			m_Model->over = false;
		}
	}
}

void MeshFilter::SetFBX(bool value)
{
	fbx = value;

	m_Model = nullptr;
	m_Model_obj = nullptr;

	//if (fbx == true)
	//{
	//	if (m_Model_obj != nullptr)
	//	{
	//		m_Model_obj = nullptr;
	//	}

	//	m_Model = nullptr;
	//	m_Model = new AnimationModel();
	//	m_Model->over = false;

	//}
	//else
	//{
	//	if (m_Model != nullptr)
	//	{
	//		m_Model = nullptr;
	//	}

	//	m_Model_obj = nullptr;
	//	m_Model_obj = new Model();
	//}
}

void MeshFilter::SetModel(ModelReader::READ_MODEL_FBX index) 
{ 
	SetFBX(true); 
	m_Model = ModelReader::GetReadModel(index);
	m_Model->over = false;
	
	modelIndex = ModelReader::READ_MODEL_OBJ_MAX + index; 
}

void MeshFilter::SetModel(ModelReader::READ_MODEL_OBJ index)
{
	SetFBX(false);
	m_Model_obj = ModelReader::GetReadModel(index);

	modelIndex = index;
}