#include "script.h"
#include "manager.h"
#include "input.h"

void MapCollision::Start()
{
	ignore = true;
	ignoreSize = 75.0f;
}

void MapCollision::End()
{

}

void MapCollision::Update()
{

	//直方体
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
			D3DXVECTOR3 right = gameObject->transform->GetRightDirection();        //X分離軸
			D3DXVECTOR3 forward = gameObject->transform->GetForwardDirection();    //Z分離軸
			D3DXVECTOR3 direction = boxPos - object->transform->Position;    //直方体からプレイヤーまでの方向ベクトル

			if (D3DXVec3Length(&direction) > ignoreSize && ignore == true) { continue; }

			float obbx = D3DXVec3Dot(&direction, &right);    //X分離軸方向プレイヤー距離
			float obbz = D3DXVec3Dot(&direction, &forward);    //Z分離軸方向プレイヤー距離

			float overlapX = scale.x - fabs(obbx) + object->transform->Scale.x;
			float overlapZ = scale.z - fabs(obbz) + object->transform->Scale.x;

			// オーバーラップが発生した場合
			if (overlapX > 0.0f && overlapZ > 0.0f)
			{
				BulletScript* script = object->GetComponent<BulletScript>();
				if (script)
				{
					script->OnDestruction(false);
					continue; 
				}

				D3DXVECTOR3 pushDirection(0.0f, 0.0f, 0.0f);
				if (overlapX < overlapZ)
				{
					pushDirection = obbx / fabs(obbx) * -right;
					object->transform->Position += pushDirection * (overlapX);

				}
				else
				{
					pushDirection = obbz / fabs(obbz) * -forward;
					object->transform->Position += pushDirection * (overlapZ);
				}

				ArtificialIntelligence* ai = object->GetComponent<ArtificialIntelligence>();
				if (ai)
				{
					ai->SetStateToReturn();
					continue;
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
		DebugManager::BoolDisplay(&ignore, -200.0f, "Ignore", 1);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}