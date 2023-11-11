#include "../OBJECT STORE/component.h"
#include "../manager.h"
#include "../MANAGEMENT FUNCTIONS/scene.h"

void Camera::Start()
{
	shake = false;

	shakeCounter = 0;

	fov = 60.0f;
	len = 0.0f;
	rad = 0.0f;
	shakeValue = 0.0f;

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
		shakeCounter++;
		if (shakeCounter % 3 == 0) { shakeValue *= -1; }
		at.x += shakeValue;

		if (shakeCounter % 15 == 0) { shake = false; }
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

void Camera::CameraShake(float value)
{
	if (shake == false)
	{
		shake = true;
		shakeCounter = 0;
		shakeValue = value * 2;
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