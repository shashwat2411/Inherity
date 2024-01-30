#include "script.h"
#include "manager.h"
#include "input.h"

void MapCollision::Start()
{

}

void MapCollision::End()
{

}

void MapCollision::Update()
{
	//������
	{
		std::vector<ENEMY*> enemies = Manager::GetScene()->FindGameObjects<ENEMY>();
		std::vector<BULLET*> bullets = Manager::GetScene()->FindGameObjects<BULLET>();
		PLAYER* player = Manager::GetScene()->FindGameObject<PLAYER>();

		std::vector<GAMEOBJECT*> objects;
		objects.insert(objects.end(), enemies.begin(), enemies.end());
		objects.insert(objects.end(), bullets.begin(), bullets.end());
		objects.push_back(player);


		for (GAMEOBJECT* object : objects)
		{

			D3DXVECTOR3 boxPos = gameObject->transform->Position;
			D3DXVECTOR3 scale = gameObject->transform->Scale * 1.3f;
			D3DXVECTOR3 right = gameObject->transform->GetRightDirection();        //X������
			D3DXVECTOR3 forward = gameObject->transform->GetForwardDirection();    //Z������
			D3DXVECTOR3 direction = boxPos - object->transform->Position;    //�����̂���v���C���[�܂ł̕����x�N�g��


			float obbx = D3DXVec3Dot(&direction, &right);    //X�����������v���C���[����
			float obbz = D3DXVec3Dot(&direction, &forward);    //Z�����������v���C���[����

			float overlapX = scale.x - fabs(obbx);
			float overlapZ = scale.z - fabs(obbz);

			// �I�[�o�[���b�v�����������ꍇ
			if (overlapX > 0.0f && overlapZ > 0.0f)
			{
				if (object->GetComponent<BulletScript>()) { object->Destroy(); continue; }

				D3DXVECTOR3 pushDirection(0.0f, 0.0f, 0.0f);
				if (overlapX < overlapZ)
				{
					pushDirection = obbx / fabs(obbx) * -right;
					object->transform->Position += pushDirection * overlapX;

				}
				else
				{
					pushDirection = obbz / fabs(obbz) * -forward;
					object->transform->Position += pushDirection * overlapZ;
				}
			}
		}
	}
}

void MapCollision::Draw()
{

}

void MapCollision::EngineDisplay()
{
	if (ImGui::TreeNode("Map Collision"))
	{
		ImGui::TreePop();
		ImGui::Spacing();
	}
}