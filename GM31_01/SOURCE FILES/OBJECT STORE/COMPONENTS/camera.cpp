#include "component.h"
#include "manager.h"

float cameraShakeValueGlobal = 0.0f;
float cameraShakeLimitGlobal = 15.0f / FRAME_RATE;
bool cameraShakeBoolGlobal = false;

void Camera::Start()
{
	shake = false;

	power = 1;

	fov = 60.0f;
	len = 0.0f;
	rad = 0.0f;
	
	limit = 15.0f / FRAME_RATE;
	shakeCounter = 0.0f;
	shakeValue = 0.0f;
	time = 0.0f;

	Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	at = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	//----------------------------------------------------------------
	float vx, vz;
	vx = gameObject->transform->Position.x - at.x;
	vz = gameObject->transform->Position.z - at.z;
	len = sqrtf(vx * vx + vz * vz);

	gameObject->transform->Position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);

}

void Camera::End()
{
}

void Camera::Update()
{
}

void Camera::Draw()
{
	if (shake == true)
	{
		shakeCounter += Time::deltaTime;
		time += Time::deltaTime;
		if (shakeCounter >= 3.0f / FRAME_RATE) { power *= -1; shakeCounter = 0.0f; }
		at.x += shakeValue * power * Time::fixedTimeScale;

		if (time >= limit) { shake = false; cameraShakeBoolGlobal = false; }
	}


	//FOV Matrix
	{
		D3DXMatrixLookAtLH(&mtxView, &gameObject->transform->Position, &at, &Up);
		Renderer::SetViewMatrix(&mtxView);

		D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);
		Renderer::SetProjectionMatrix(&mtxProjection);


		Renderer::SetCameraPosition(gameObject->transform->Position);
	}
}

void Camera::EngineDisplay()
{
	if (ImGui::TreeNode("Camera"))
	{
		ImGui::Text("Target : %s", (Target ? Target->GetTag().c_str() : "nullptr"));


		DebugManager::FloatDisplay(&fov, -FLT_MIN, "FOV", false, D3DXVECTOR2(1.0f, 120.0f), 0);
		//ImGui::PushItemWidth(-FLT_MIN);
		//char str[14];
		//sprintf_s(str, sizeof(str),"FOV : %.2f", fov);
		//ImGui::SliderFloat(" ", &fov, 1.0f, 120.0f, str);

		if (DebugManager::BoolDisplay(&shake, -200.0f, "Shake", 1)) { 
			CameraShake(cameraShakeValueGlobal, cameraShakeLimitGlobal); 
		}

		if (ImGui::TreeNode("Details"))
		{
			//char str[22];

			//ImGui::PushItemWidth(-FLT_MIN);
			//sprintf_s(str, sizeof(str), "Shake Value : %.2f", cameraShakeValueGlobal);
			//ImGui::DragFloat(" ", &cameraShakeValueGlobal, 0.01F, 0.0F, 0.0F, str);
			DebugManager::FloatDisplay(&cameraShakeValueGlobal, -FLT_MIN, "Shake Value", true, D3DXVECTOR2(0.01f, 0.0f), 2);
			DebugManager::FloatDisplay(&cameraShakeLimitGlobal, -FLT_MIN, "Shake Time", false, D3DXVECTOR2(15.0f / FRAME_RATE, 1.0f), 3);

			//ImGui::PushItemWidth(-FLT_MIN);
			//sprintf_s(str, sizeof(str), "Shake Time : %.2f", cameraShakeLimitGlobal);
			//ImGui::SliderFloat("  ", &cameraShakeLimitGlobal, 15.0f / FRAME_RATE, 1.0f, str);

			ImGui::Text("Target Transform");
			Target->transform->EngineDisplay();

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

D3DXVECTOR3 Camera::GetForward()
{
	D3DXMATRIX rot;
	D3DXMatrixInverse(&rot, nullptr, &mtxView);

	D3DXVECTOR3 forward;
	forward.x = rot._31;
	forward.y = rot._32;
	forward.z = rot._33;

	return forward;
}

D3DXVECTOR3 Camera::GetRight()
{
	D3DXMATRIX rot;
	D3DXMatrixInverse(&rot, nullptr, &mtxView);

	D3DXVECTOR3 right;
	right.x = rot._11;
	right.y = rot._12;
	right.z = rot._13;

	return right;
}

void Camera::CameraShake(float value, float t)
{
	if (cameraShakeBoolGlobal == false)
	{
		cameraShakeBoolGlobal = true;
		shake = true;
		limit = t;
		shakeCounter = 0.0f;
		shakeValue = value;
		time = 0.0f;
	}
}

bool Camera::CheckView(Transform* target)
{
	if (target->culling == false) { return true; }

	D3DXMATRIX vp;
	D3DXMATRIX invvp;

	vp = mtxView * mtxProjection;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vPos[4];
	D3DXVECTOR3 wPos[4];

	vPos[0] = D3DXVECTOR3( -1.0f,  1.0f,  1.0f);
	vPos[1] = D3DXVECTOR3(  1.0f,  1.0f,  1.0f);
	vPos[2] = D3DXVECTOR3( -1.0f, -1.0f,  1.0f);
	vPos[3] = D3DXVECTOR3(  1.0f, -1.0f,  1.0f);

	D3DXVec3TransformCoord(&wPos[0], &vPos[0], &invvp);
	D3DXVec3TransformCoord(&wPos[1], &vPos[1], &invvp);
	D3DXVec3TransformCoord(&wPos[2], &vPos[2], &invvp);
	D3DXVec3TransformCoord(&wPos[3], &vPos[3], &invvp);


	D3DXVECTOR3 v;
	D3DXVECTOR3 v1;
	D3DXVECTOR3 v2;
	D3DXVECTOR3 n;
	D3DXVECTOR3 position;

	position = gameObject->transform->Position;
	v = target->GlobalPosition - position;

	//���ʔ���
	{
		v1 = wPos[0] - position;
		v2 = wPos[2] - position;
		D3DXVec3Cross(&n, &v1, &v2);
		D3DXVec3Normalize(&n, &n);

		if (D3DXVec3Dot(&n, &v) < -target->drawRadius)
		{
			return false;
		}
	}

	//�E�ʔ���
	{
		v1 = wPos[3] - position;
		v2 = wPos[1] - position;
		D3DXVec3Cross(&n, &v1, &v2);
		D3DXVec3Normalize(&n, &n);

		if (D3DXVec3Dot(&n, &v) < -target->drawRadius)
		{
			return false;
		}
	}

	//��ʔ���
	{
		v1 = wPos[1] - position;
		v2 = wPos[0] - position;
		D3DXVec3Cross(&n, &v1, &v2);
		D3DXVec3Normalize(&n, &n);

		if (D3DXVec3Dot(&n, &v) < -target->drawRadius)
		{
			return false;
		}
	}

	//���ʔ���
	{
		v1 = wPos[2] - position;
		v2 = wPos[3] - position;
		D3DXVec3Cross(&n, &v1, &v2);
		D3DXVec3Normalize(&n, &n);

		if (D3DXVec3Dot(&n, &v) < -target->drawRadius)
		{
			return false;
		}
	}

	////�O�ʔ���
	//{
	//	n = gameObject->transform->GetForwardDirection();
	//	D3DXVec3Normalize(&n, &n);

	//	if (D3DXVec3Dot(&n, &v) < -target->drawRadius)
	//	{
	//		return false;
	//	}
	//}

	////��ʔ���
	//{
	//	n = -gameObject->transform->GetForwardDirection();
	//	D3DXVec3Normalize(&n, &n);

	//	if (D3DXVec3Dot(&n, &v) < -target->drawRadius)
	//	{
	//		return false;
	//	}
	//}

	return true;
}