#include "component.h"
#include "input.h"

float rotation = 0.0f;

void RevolutionCamera::Update()
{

	if (Input::GetKeyPress('Q')) { rotation += 0.1f; }
	if (Input::GetKeyPress('E')) { rotation -= 0.1f; }

	//gameObject->transform->Position = Target->transform->Position + D3DXVECTOR3(sinf(rotation) * 5.0f, 4.0f, cosf(rotation) * 5.0f);
	//at = Target->transform->Position;



	D3DXVECTOR3 toBeAt = Target->transform->GlobalPosition - Target->Children[0]->transform->GetForwardDirection() * 8.0f + D3DXVECTOR3(0.0f, 4.0f, 0.0f);
	D3DXVECTOR3 toLookAt = Target->transform->GlobalPosition;


	D3DXVECTOR3 AtVec;
	D3DXVECTOR3 PosVec;

	//今回のカメラの座標
	PosVec.x = toBeAt.x - sinf(rot.y) * len;
	PosVec.y = toBeAt.y;
	PosVec.z = toBeAt.z - cosf(rot.y) * len;

	AtVec = toLookAt - at;	//注視点の変化ベクトル
	PosVec = PosVec - gameObject->transform->Position;	//一座標の変化

	PosVec *= 0.008f;	//ベクトルスケーリング
	AtVec *= 0.08f;	//ベクトルスケーリング

	// カメラの注視点をプレイヤーの座標にしてみる
	at += AtVec * Time::fixedTimeScale;
	gameObject->transform->Position += PosVec * Time::fixedTimeScale;

}