#include "script.h"
#include "../saveFunctions.h"
#include "manager.h"

float rotation = 0.0f;
D3DXVECTOR3 direction;

void RevolutionCamera::Update()
{
	//if (Input::GetKeyPress('Q')) { rotation += 0.1f; }
	//if (Input::GetKeyPress('E')) { rotation -= 0.1f; }

	//gameObject->transform->Position = Target->transform->Position + D3DXVECTOR3(sinf(rotation) * 5.0f, 4.0f, cosf(rotation) * 5.0f);
	//at = Target->transform->Position;

	targetOffset.x = /*ImGui::GetMousePos().x*/ /*GetMousePosX()*/ /*Input::CameraAngleHorizontal()*/aimer->transform->Position.x - SCREEN_WIDTH / 2;
	targetOffset.y = -(/*ImGui::GetMousePos().y*/ /*GetMousePosY()*/ /*Input::CameraAngleVertical()*/aimer->transform->Position.y - SCREEN_HEIGHT / 2);
	targetOffset.z = 0.0f;

	if (fabs(targetOffset.x) > screenLimit.x) { targetOffset.x = targetOffset.x / fabs(targetOffset.x) * screenLimit.x; }
	if (targetOffset.y > screenLimit.y) { targetOffset.y = screenLimit.y; }
	else if (targetOffset.y < screenLimit.z) { targetOffset.y = screenLimit.z; }

	direction = camera->GetUp() * targetOffset.y + camera->GetRight() * targetOffset.x;


	D3DXVECTOR3 forward, right;
	if ((int)camera->GetTarget()->GetChildren().size() > 0) 
	{
		forward = camera->GetTarget()->GetChildren()[0]->transform->GetForwardDirection(); 
		right = camera->GetTarget()->GetChildren()[0]->transform->GetRightDirection(); 
	}
	else 
	{ 
		forward = camera->GetTarget()->transform->GetForwardDirection(); 
		right = camera->GetTarget()->transform->GetRightDirection();
	}

	D3DXVECTOR3 distance = -forward * backUpDistance.z + right * backUpDistance.x + D3DXVECTOR3(0.0f, backUpDistance.y, 0.0f);

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

	PosVec *= followSpeed.x * (gameObject->transform->DistanceFrom(camera->GetTarget()) < 2.0f ? 5.0f : 1.0f);	//ベクトルスケーリング
	AtVec *= followSpeed.y;	//ベクトルスケーリング

	// カメラの注視点をプレイヤーの座標にしてみる
	at += (AtVec + right * backUpDistance.x) * Time::fixedTimeScale;

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
		DebugManager::Float3Display(&backUpDistance, -1.0f, "Distance", 0.05f, 3, false);
		DebugManager::Float3Display(&followSpeed, -1.0f, "Follow", 0.01f, 4, false);
		
		ImGui::TreePop();
		ImGui::Spacing();
	}
}