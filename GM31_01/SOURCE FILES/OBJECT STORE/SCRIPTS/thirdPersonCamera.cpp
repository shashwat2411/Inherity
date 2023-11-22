#include "script.h"

void ThirdPersonCamera::Update()
{
	if (camera->Target != nullptr)
	{

		//D3DXVECTOR3 Focus = Target->transform->Position + gameObject->GetRightDirection() * 0.5f;
		D3DXVECTOR3 toBeAt = camera->Target->transform->GlobalPosition - camera->Target->Children[0]->transform->GetForwardDirection() * 8.0f + D3DXVECTOR3(0.0f, 4.0f, 0.0f);
		//D3DXVECTOR3 toBeAt = Target->transform->GlobalPosition - Target->Children[0]->transform->GetForwardDirection() * 12.0f + D3DXVECTOR3(0.0f, 8.0f, 0.0f);
		D3DXVECTOR3 toLookAt = camera->Target->transform->GlobalPosition;
		//D3DXVECTOR3 toBeAt = at - Target->transform->GetForwardDirection() * 1.0f + D3DXVECTOR3asds(0.0f, 0.0f, 0.0f);


		D3DXVECTOR3 AtVec;
		D3DXVECTOR3 PosVec;
		D3DXVECTOR3 at = camera->GetAt();

		//����̃J�����̍��W
		PosVec.x = toBeAt.x - sinf(camera->GetRot().y) * camera->GetLen();
		PosVec.y = toBeAt.y;
		PosVec.z = toBeAt.z - cosf(camera->GetRot().y) * camera->GetLen();

		AtVec = toLookAt - at;	//�����_�̕ω��x�N�g��
		PosVec = PosVec - gameObject->transform->Position;	//����W�̕ω�

		PosVec *= 0.008f;	//�x�N�g���X�P�[�����O
		AtVec *= 0.08f;	//�x�N�g���X�P�[�����O

		// �J�����̒����_���v���C���[�̍��W�ɂ��Ă݂�
		at += AtVec * Time::fixedTimeScale;

		camera->SetAt(at);
		gameObject->transform->Position += PosVec * Time::fixedTimeScale;
	}
}