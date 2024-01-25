#include "component.h"
#include "manager.h"

void Projector::Start()
{
	faced = false;

	fov = 60.0f;
	fovNear = 0.1f;
	fovFar = 10.0f;

	Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	at = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	direction = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Target = nullptr;

	parentPoint = new EMPTYOBJECT();
	parentPoint->Init();

	point = new EMPTYOBJECT();
	point->Init();

	targetName = "nullptr";

	point->Parent = parentPoint;
	point->transform->Position.z = 2.0f;
	point->transform->Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);

#ifdef DEBUG
	point->AddComponent<MeshFilter>()->SetModel(ModelReader::SPHERE_COLLIDER_M);
#endif
}

void Projector::End()
{
	point->UnInitialize();
	parentPoint->UnInitialize();

	delete point;
	delete parentPoint;
}

void Projector::Update()
{
	GAMEOBJECT* targeted = Manager::GetScene()->Find(targetName);

	if (Target == nullptr)
	{
		if (CheckView(targeted->transform))
		{
			Target = targeted;
		}
	}
	else if (parentPoint->transform->DistanceFrom(Target) > fovFar)
	{
		Target = nullptr;
	}
}

void Projector::Draw()
{
	if (faced == false)
	{
		parentPoint->transform->Position = gameObject->transform->Position;

		if (Target != nullptr) 
		{ 
			parentPoint->transform->FaceTowards(Target); 
			gameObject->transform->Rotation = parentPoint->transform->Rotation;
		}
		else { parentPoint->transform->Rotation = direction + gameObject->transform->Rotation; }

		parentPoint->Update();
		point->Update();

		parentPoint->Draw();
		point->Draw();

		at = point->transform->GlobalPosition;
	}


	//FOV Matrix
	{
		D3DXMatrixLookAtLH(&mtxView, &gameObject->transform->Position, &at, &Up);

		D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, fovNear, fovFar);
	}
}

void Projector::EngineDisplay()
{
	if (ImGui::TreeNode("Projector"))
	{
		ImGui::Text("Target : %s", (Target ? Target->GetTag().c_str() : "nullptr"));

		DebugManager::Float3Display(&direction, -1.0f, "Direction ", 0.5f, 1);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

bool Projector::CheckView(Transform* target)
{
	D3DXMATRIX vp;
	D3DXMATRIX invvp;

	vp = mtxView * mtxProjection;
	D3DXMatrixInverse(&invvp, NULL, &vp);

	D3DXVECTOR3 vPos[4];
	D3DXVECTOR3 wPos[4];

	vPos[0] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	vPos[1] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vPos[2] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	vPos[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);

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

	//ç∂ñ îªíË
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

	//âEñ îªíË
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

	//è„ñ îªíË
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

	//â∫ñ îªíË
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

	////ëOñ îªíË
	//{
	//	n = gameObject->transform->GetForwardDirection();
	//	D3DXVec3Normalize(&n, &n);

	//	if (D3DXVec3Dot(&n, &v) < -target->drawRadius)
	//	{
	//		return false;
	//	}
	//}
	if (parentPoint->transform->DistanceFrom(target->gameObject) > fovFar)
	{
		return false;
	}

	////å„ñ îªíË
	//{
	//	n = -gameObject->transform->GetForwardDirection();
	//	D3DXVec3Normalize(&n, &n);

	//	if (D3DXVec3Dot(&n, &v) < -target->drawRadius)
	//	{
	//		return false;
	//	}
	//}
	if (parentPoint->transform->DistanceFrom(target->gameObject) < fovNear)
	{
		return false;
	}

	return true;
}