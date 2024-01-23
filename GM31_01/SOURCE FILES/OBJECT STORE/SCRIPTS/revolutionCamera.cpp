#include "script.h"
#include "../saveFunctions.h"

float rotation = 0.0f;
D3DXVECTOR3 direction;

void RevolutionCamera::Update()
{
	//if (Input::GetKeyPress('Q')) { rotation += 0.1f; }
	//if (Input::GetKeyPress('E')) { rotation -= 0.1f; }

	//gameObject->transform->Position = Target->transform->Position + D3DXVECTOR3(sinf(rotation) * 5.0f, 4.0f, cosf(rotation) * 5.0f);
	//at = Target->transform->Position;

	targetOffset.x = ImGui::GetMousePos().x - SCREEN_WIDTH / 2;
	targetOffset.y = -(ImGui::GetMousePos().y - SCREEN_HEIGHT / 2);
	targetOffset.z = 0.0f;

	if (fabs(targetOffset.x) > 300.0f) { targetOffset.x = targetOffset.x / fabs(targetOffset.x) * 300.0f; }
	if (targetOffset.y > 180.0f) { targetOffset.y = 180.0f; }
	else if (targetOffset.y < -40.0f) { targetOffset.y = -40.0f; }

	direction = camera->GetUp() * targetOffset.y + camera->GetRight() * targetOffset.x;


	D3DXVECTOR3 forward;
	if ((int)camera->GetTarget()->GetChildren().size() > 0) { forward = camera->GetTarget()->GetChildren()[0]->transform->GetForwardDirection(); }
	else { forward = camera->GetTarget()->transform->GetForwardDirection(); }

	D3DXVECTOR3 distance = -forward * 8.0f + D3DXVECTOR3(0.0f, 4.0f, 0.0f);

	D3DXVECTOR3 toBeAt = camera->GetTarget()->transform->GlobalPosition + distance;
	D3DXVECTOR3 toLookAt = camera->GetTarget()->transform->GlobalPosition + direction * offsetSpeed;


	D3DXVECTOR3 AtVec;
	D3DXVECTOR3 PosVec;
	D3DXVECTOR3 at = camera->GetAt();

	//今回のカメラの座標
	PosVec.x = toBeAt.x - sinf(camera->GetRot().y) * camera->GetLen();
	PosVec.y = toBeAt.y;
	PosVec.z = toBeAt.z - cosf(camera->GetRot().y) * camera->GetLen();

	AtVec = toLookAt - at;	//注視点の変化ベクトル
	PosVec = PosVec - gameObject->transform->Position;	//一座標の変化

	PosVec *= 0.012f;	//ベクトルスケーリング
	AtVec *= 0.08f;	//ベクトルスケーリング

	// カメラの注視点をプレイヤーの座標にしてみる
	at += (AtVec/* + targetOffset * offsetSpeed*/) * Time::fixedTimeScale;

	camera->SetAt(at);
	gameObject->transform->Position += PosVec * Time::fixedTimeScale;

}

void RevolutionCamera::EngineDisplay()
{
	if (ImGui::TreeNode("Revolution Camera"))
	{
		DebugManager::FloatDisplay(&offsetSpeed, -FLT_MIN, "Offset Speed", true, D3DXVECTOR2(0.1f, 0.0f), 0);

		DebugManager::Float3Display(&direction, -1.0f, "Direction ", 0.05f, 1, true);
		DebugManager::Float3Display(&targetOffset, -1.0f, "Offset ", 0.05f, 2, true);
		
		ImGui::TreePop();
		ImGui::Spacing();
	}
}