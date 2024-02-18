#include "script.h"
#include "manager.h"

//#define CONTROLLER

void ScreenToWorld::Start()
{
	speed = 8.0f;
	aimSpeed = 4.0f;

	point = Manager::GetScene()->AddGameObject<EMPTYOBJECT>("3D Point", GIZMO_LAYER);
	point->AddComponent<MeshFilter>()->SetModel(ModelReader::SPHERE_COLLIDER_M);

	screenPosition = D3DXVECTOR2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
}

void ScreenToWorld::End()
{

}

void ScreenToWorld::Update()
{
	if (Input::GetControllerConnection() == true)
	{
		float finalSpeed = 0.0f;

		if (Manager::GetScene()->GetPlayer()->GetComponent<PlayerMovement>()->GetState() == PlayerMovement::AIMING_MOVE_PS) { finalSpeed = aimSpeed; }
		else { finalSpeed = speed; }

		screenPosition += D3DXVECTOR2(Input::CameraAngleHorizontal(), -Input::CameraAngleVertical()) * finalSpeed * Time::fixedTimeScale;

		if (screenPosition.x >= SCREEN_WIDTH - 1) { screenPosition.x = SCREEN_WIDTH - 1; }
		else if (screenPosition.x <= 0.0f) { screenPosition.x = 0.0f; }
		if (screenPosition.y >= SCREEN_HEIGHT - 1) { screenPosition.y = SCREEN_HEIGHT - 1; }
		else if (screenPosition.y <= 0.0f) { screenPosition.y = 0.0f; }
	}
	else
	{
		screenPosition = D3DXVECTOR2(float(GetMousePosX()), float(GetMousePosY()));

		if (screenPosition.x >= SCREEN_WIDTH - 1) { screenPosition.x = SCREEN_WIDTH - 1; }
		if (screenPosition.x <= 1.0f) { screenPosition.x = 1.0f; }
		if (screenPosition.y >= SCREEN_HEIGHT - 1) { screenPosition.y = SCREEN_HEIGHT - 1; }
		if (screenPosition.y <= 1.0f) { screenPosition.y = 1.0f; }
	}

	gameObject->transform->Position = D3DXVECTOR3(screenPosition.x, screenPosition.y, 0.0f);

	CalcScreenToXZ(
		&worldPosition,
		gameObject->transform->Position.x,
		gameObject->transform->Position.y,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		&Manager::GetScene()->GetCamera()->camera->GetViewMatrix(),
		&Manager::GetScene()->GetCamera()->camera->GetProjectionMatrix()
	);

	point->transform->Position = worldPosition;
}

void ScreenToWorld::Draw()
{

}

void ScreenToWorld::EngineDisplay()
{
	if (ImGui::TreeNode("Screen To World"))
	{
		DebugManager::FloatDisplay(&speed, -FLT_MIN, "Speed", true, D3DXVECTOR2(0.01f, 0.0f), 0);
		DebugManager::FloatDisplay(&aimSpeed, -FLT_MIN, "Aim Speed", true, D3DXVECTOR2(0.01f, 0.0f), 1);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

D3DXVECTOR3* ScreenToWorld::CalcScreenToWorld(
	D3DXVECTOR3* pout,
	int Sx,  // スクリーンX座標
	int Sy,  // スクリーンY座標
	float fZ,  // 射影空間でのZ値（0～1）
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* View,
	D3DXMATRIX* Prj
) {
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return pout;
}

// XZ平面とスクリーン座標の交点算出関数
D3DXVECTOR3* ScreenToWorld::CalcScreenToXZ(
	D3DXVECTOR3* pout,
	int Sx,
	int Sy,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* View,
	D3DXMATRIX* Prj
) {
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// 床との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	if (ray.y <= 0) {
		// 床交点
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray)*ray;
	}
	else {
		*pout = farpos;
	}

	return pout;
}