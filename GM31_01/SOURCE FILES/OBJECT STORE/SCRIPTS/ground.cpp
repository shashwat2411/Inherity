#include "script.h"
#include "manager.h"

void Ground::Start()
{

}

void Ground::End()
{

}

void Ground::Update()
{
	{
		for (auto cylinder : Manager::GetScene()->FindGameObjects<CYLINDER>())
		{
			if (gameObject->transform->DistanceFromWithY0(cylinder) < cylinder->transform->Scale.x + gameObject->transform->Scale.x)
			{
				if (gameObject->transform->Position.y < cylinder->transform->Position.y + cylinder->transform->Scale.y - 0.5f)
				{
					gameObject->transform->Position.x = gameObject->transform->Position.x - gameObject->rigidbody->Speed.x;
					gameObject->transform->Position.z = gameObject->transform->Position.z - gameObject->rigidbody->Speed.z;
				}
				else
				{
					gameObject->rigidbody->groundLevel = cylinder->transform->Position.y + cylinder->transform->Scale.y + 1.0f;
				}
				break;
			}
			else
			{
				gameObject->rigidbody->groundLevel = 0.0f;
			}
		}
	}

	{
		for (auto box : Manager::GetScene()->FindGameObjects<CUBE>())
		{
			D3DXVECTOR3 playerPosition = gameObject->transform->Position;
			D3DXVECTOR3 playerScale = gameObject->transform->Scale;
			D3DXVECTOR3 boxPosition = box->transform->Position;
			D3DXVECTOR3 boxScale = box->transform->Scale;

			if (playerPosition.x < boxPosition.x + boxScale.x + 0.5f && playerPosition.x > boxPosition.x - boxScale.x - 0.5f)
			{
				if (playerPosition.z< boxPosition.z + boxScale.z + 0.5f && playerPosition.z > boxPosition.z - boxScale.z - 0.5f)
				{
					if (playerPosition.y < boxPosition.y + boxScale.y * 2.0f - 0.5f)
					{
						gameObject->transform->Position.x = gameObject->transform->Position.x - gameObject->rigidbody->Speed.x;
						gameObject->transform->Position.z = gameObject->transform->Position.z - gameObject->rigidbody->Speed.z;
					}
					else
					{
						gameObject->rigidbody->groundLevel = box->transform->Position.y + box->transform->Scale.y * 2.0f + 1.0f;
					}
					break;
				}
			}
		}
	}
}

void Ground::Draw()
{
	
}

void Ground::EngineDisplay()
{
	if (ImGui::TreeNode("Ground"))
	{
		ImGui::TreePop();
		ImGui::Spacing();
	}
}