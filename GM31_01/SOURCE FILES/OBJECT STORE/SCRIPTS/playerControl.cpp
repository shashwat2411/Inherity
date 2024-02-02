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
	////������
	//{
	//	GAMEOBJECT* box = Manager::GetScene()->Find("Cube(Clone)");

	//	SphereCollider* collider = gameObject->GetComponent<SphereCollider>();
	//	if (collider == nullptr)
	//	{
	//		collider = gameObject->GetComponentInChildren<SphereCollider>();
	//	}
	//	float objectScale = collider->GetCollisionSize() / 2.0f;

	//	D3DXVECTOR3 boxPos = box->transform->Position;
	//	D3DXVECTOR3 scale = box->transform->Scale * 1.3f;
	//	D3DXVECTOR3 right = box->transform->GetRightDirection();        //X������
	//	D3DXVECTOR3 forward = box->transform->GetForwardDirection();    //Z������
	//	D3DXVECTOR3 direction = boxPos - gameObject->transform->Position;    //�����̂���v���C���[�܂ł̕����x�N�g��


	//	float obbx = D3DXVec3Dot(&direction, &right);    //X�����������v���C���[����
	//	float obbz = D3DXVec3Dot(&direction, &forward);    //Z�����������v���C���[����

	//	float overlapX = scale.x - fabs(obbx);
	//	float overlapZ = scale.z - fabs(obbz);

	//	// �I�[�o�[���b�v�����������ꍇ
	//	if (overlapX > 0.0f && overlapZ > 0.0f)
	//	{
	//		D3DXVECTOR3 pushDirection(0.0f, 0.0f, 0.0f);
	//		if (overlapX < overlapZ)
	//		{
	//			pushDirection = obbx / fabs(obbx) * -right;
	//			gameObject->transform->Position += pushDirection * overlapX *  Time::deltaTime * 8.0f;

	//		}
	//		else
	//		{
	//			pushDirection = obbz / fabs(obbz) * -forward;
	//			gameObject->transform->Position += pushDirection * overlapZ *  Time::deltaTime * 8.0f;
	//		}
	//	}
	//}

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