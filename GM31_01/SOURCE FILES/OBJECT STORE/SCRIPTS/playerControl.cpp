#include "script.h"
#include "manager.h"
#include "input.h"

void PlayerControl::Start()
{
	timerVector["_Counter"] = 0.0f;
}

void PlayerControl::End()
{

}

void PlayerControl::Update()
{
	//直方体
	{
		GAMEOBJECT* box = Manager::GetScene()->Find("Cube(Clone)");

		D3DXVECTOR3 boxPos = box->transform->Position;
		D3DXVECTOR3 scale = box->transform->Scale * 1.3f;
		D3DXVECTOR3 right = box->transform->GetRightDirection();        //X分離軸
		D3DXVECTOR3 forward = box->transform->GetForwardDirection();    //Z分離軸
		D3DXVECTOR3 direction = boxPos - gameObject->transform->Position;    //直方体からプレイヤーまでの方向ベクトル


		float obbx = D3DXVec3Dot(&direction, &right);    //X分離軸方向プレイヤー距離
		float obbz = D3DXVec3Dot(&direction, &forward);    //Z分離軸方向プレイヤー距離

		float overlapX = scale.x - fabs(obbx);
		float overlapZ = scale.z - fabs(obbz);

		float friction = 0.9f;

		// オーバーラップが発生した場合
		if (overlapX > 0.0f && overlapZ > 0.0f)
		{
			//D3DXVECTOR3 diffe = m_OldPosition - gameObject->transform->Position;
			//gameObject->transform->Position += friction * diffe;

			//direction = boxPos - gameObject->transform->Position;    //直方体からプレイヤーまでの方向ベクトル

			//float obbx = D3DXVec3Dot(&direction, &right);    //X分離軸方向プレイヤー距離
			//float obbz = D3DXVec3Dot(&direction, &forward);    //Z分離軸方向プレイヤー距離

			//float overlapX = scale.x - fabs(obbx);
			//float overlapZ = scale.z - fabs(obbz);

			// どちらがオーバーラップしているかによって反発方向を決定
			D3DXVECTOR3 pushDirection(0.0f, 0.0f, 0.0f);
			if (overlapX < overlapZ)
			{
				pushDirection = obbx / fabs(obbx) * -right;
				gameObject->transform->Position += pushDirection * overlapX;

			}
			else
			{
				pushDirection = obbz / fabs(obbz) * -forward;
				gameObject->transform->Position += pushDirection * overlapZ;
			}
		}
	}

	if (Input::GetKeyPress('R'))
	{
		Spawn();
	}
}

void PlayerControl::Draw()
{

}

void PlayerControl::EngineDisplay()
{
	if (ImGui::TreeNode("Player Control"))
	{
		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void PlayerControl::Spawn()
{
	if (Time::WaitForSeconds(2.0f, &timerVector["_Counter"]) == false) { return; }

	timerVector["_Counter"] = 0.0f;
	Manager::GetScene()->AddGameObject<ROCK>()->transform->Position = gameObject->transform->Position;
}