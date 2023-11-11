#include "../OBJECT STORE/component.h"

void ThirdPersonCamera::Update()
{
	if (Target != nullptr)
	{

		//D3DXVECTOR3 Focus = Target->transform->Position + gameObject->GetRightDirection() * 0.5f;
		//D3DXVECTOR3 toBeAt = Target->transform->GlobalPosition - Target->Children[0]->transform->GetForwardDirection() * 8.0f + D3DXVECTOR3(0.0f, 4.0f, 0.0f);
		D3DXVECTOR3 toBeAt = Target->transform->GlobalPosition - Target->Children[0]->transform->GetForwardDirection() * 12.0f + D3DXVECTOR3(0.0f, 8.0f, 0.0f);
		D3DXVECTOR3 toLookAt = Target->transform->GlobalPosition;
		//D3DXVECTOR3 toBeAt = at - Target->transform->GetForwardDirection() * 1.0f + D3DXVECTOR3asds(0.0f, 0.0f, 0.0f);


		D3DXVECTOR3 AtVec;
		D3DXVECTOR3 PosVec;

		//����̃J�����̍��W
		PosVec.x = toBeAt.x - sinf(rot.y) * len;
		PosVec.y = toBeAt.y;
		PosVec.z = toBeAt.z - cosf(rot.y) * len;

		AtVec = toLookAt - at;	//�����_�̕ω��x�N�g��
		PosVec = PosVec - gameObject->transform->Position;	//����W�̕ω�

		PosVec *= 0.008f;	//�x�N�g���X�P�[�����O
		AtVec *= 0.08f;	//�x�N�g���X�P�[�����O

		// �J�����̒����_���v���C���[�̍��W�ɂ��Ă݂�
		at += AtVec;
		gameObject->transform->Position += PosVec;
	}
}