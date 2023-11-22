#include "component.h"
#include "manager.h"
#include "material.h"

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

bool MeshFilter::GetAnimationOver(const char* name)
{
	if (animationBlendName == name)
	{
		return m_Model->over;
	}

	return false;
}

void MeshFilter::SetAnimationBlend(const char* name, bool lp, float speed)
{
	if (animationBlendName != name)
	{
		loop = lp;

		//time = 0;
		blendRate = 0.0f;

		animationName = animationBlendName;
		animationBlendName = name;

		m_Model->over = false;
	}
}

void MeshFilter::SetFBX(bool value)
{
	fbx = value;
	if (fbx == true)
	{
		if (m_Model_obj != nullptr)
		{
			m_Model_obj = nullptr;
			delete m_Model_obj;
		}

		m_Model = nullptr;
		m_Model = new AnimationModel();
		m_Model->over = false;

	}
	else
	{
		if (m_Model != nullptr)
		{
			m_Model = nullptr;
			delete m_Model;
		}

		m_Model_obj = nullptr;
		m_Model_obj = new Model();
	}
}