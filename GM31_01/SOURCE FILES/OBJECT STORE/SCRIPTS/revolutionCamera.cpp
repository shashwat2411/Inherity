#include "script.h"
#include "../saveFunctions.h"

float rotation = 0.0f;

void RevolutionCamera::Update()
{
	//if (Input::GetKeyPress('Q')) { rotation += 0.1f; }
	//if (Input::GetKeyPress('E')) { rotation -= 0.1f; }

	//gameObject->transform->Position = Target->transform->Position + D3DXVECTOR3(sinf(rotation) * 5.0f, 4.0f, cosf(rotation) * 5.0f);
	//at = Target->transform->Position;

	targetOffset.x = ImGui::GetMousePos().x - SCREEN_WIDTH / 2;
	targetOffset.y = -(ImGui::GetMousePos().y - SCREEN_HEIGHT / 2);
	targetOffset.z = 0.0f;

	D3DXVec3Normalize(&targetOffset, &targetOffset);

	D3DXVECTOR3 forward;
	if ((int)camera->GetTarget()->GetChildren().size() > 0) { forward = camera->GetTarget()->GetChildren()[0]->transform->GetForwardDirection(); }
	else { forward = camera->GetTarget()->transform->GetForwardDirection(); }

	D3DXVECTOR3 toBeAt = camera->GetTarget()->transform->GlobalPosition - forward * 8.0f + D3DXVECTOR3(0.0f, 4.0f, 0.0f);
	D3DXVECTOR3 toLookAt = camera->GetTarget()->transform->GlobalPosition;


	D3DXVECTOR3 AtVec;
	D3DXVECTOR3 PosVec;
	D3DXVECTOR3 at = camera->GetAt();

	//����̃J�����̍��W
	PosVec.x = toBeAt.x - sinf(camera->GetRot().y) * camera->GetLen();
	PosVec.y = toBeAt.y;
	PosVec.z = toBeAt.z - cosf(camera->GetRot().y) * camera->GetLen();

	AtVec = toLookAt - at;	//�����_�̕ω��x�N�g��
	PosVec = PosVec - gameObject->transform->Position;	//����W�̕ω�

	PosVec *= 0.012f;	//�x�N�g���X�P�[�����O
	AtVec *= 0.08f;	//�x�N�g���X�P�[�����O

	// �J�����̒����_���v���C���[�̍��W�ɂ��Ă݂�
	at += (AtVec/* + targetOffset * offsetSpeed*/) * Time::fixedTimeScale;

	camera->SetAt(at);
	gameObject->transform->Position += PosVec * Time::fixedTimeScale;

}

void RevolutionCamera::EngineDisplay()
{
	if (ImGui::TreeNode("Revolution Camera"))
	{
		DebugManager::FloatDisplay(&offsetSpeed, -FLT_MIN, "Offset Speed", true, D3DXVECTOR2(0.1f, 0.0f), 0);
		
		ImGui::TreePop();
		ImGui::Spacing();
	}
}