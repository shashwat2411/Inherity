#include "../OBJECT STORE/component.h"
#include "../manager.h"
#include "../MANAGEMENT FUNCTIONS/scene.h"

void MeshFilter::Start()
{
	fbx = false;

	multiple = true;
	loop = true;

	blendRate = 0.0f;
	blendSpeed = 0.05f;
	time = 0.0f;

	gameObject->AddMaterial<DefaultMaterial>();
}

void MeshFilter::End()
{
	if (m_Model != nullptr)
	{
		m_Model = nullptr;
		delete m_Model;
	}

	if (m_Model_obj != nullptr)
	{
		m_Model_obj = nullptr;
		delete m_Model_obj;
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
			m_Model->Update(animationName.c_str(), time, animationBlendName.c_str(), time, blendRate);

			time += 1.0f;
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