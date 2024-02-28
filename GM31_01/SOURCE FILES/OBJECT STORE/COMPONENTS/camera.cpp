#include "component.h"
#include "manager.h"
#include "input.h"
#include "customScenes.h"

EMPTYOBJECT* point;
bool faced = true;

void Camera::Start()
{
	shake = false;

	power = 1;
	targetIndex = 0;

	fov = 60.0f;
	len = 0.0f;
	rad = 0.0f;
	
	//limit = 15.0f / FRAME_RATE;
	//shakeCounter = 0.0f;
	//shakeValue = 0.15f;
	//time = 0.0f;
	shakeOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	shakeTime = 0.0f;
	frequency = 1.5f;
	shakeAmplitude = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	at = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	lightDirection = D3DXVECTOR3(0.9f, -1.0f, -0.05f);

	//----------------------------------------------------------------
	float vx, vz;
	vx = gameObject->transform->Position.x - at.x;
	vz = gameObject->transform->Position.z - at.z;
	len = sqrtf(vx * vx + vz * vz);

	gameObject->transform->Position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);

	point = new EMPTYOBJECT();
	point->Init();
	point->Parent = gameObject;
	point->transform->Position.z = 2.0f;

	Target = nullptr;

	faced = false;
}

void Camera::End()
{
	point->UnInitialize();
	delete point;
}

void Camera::Update()
{
	if (DebugManager::play == true && DebugManager::paused == false)
	{
		point->Update();

		//point->transform->Position = 

		point->Draw();
	}
}

void Camera::Draw()
{
	if (LOAD_SCENE::GetLogo() == false)
	{
		if (DebugManager::play == false || DebugManager::paused == true || faced == false)
		{
			point->Update();
			point->Draw();

			at = point->transform->GlobalPosition;
			faced = true;
		}

		//Camera Shake
		{
			shakeOffset = sinf(shakeTime * FRAME_RATE * frequency) * shakeAmplitude;
			shakeTime += Time::deltaTime;
			shakeAmplitude *= 0.8f;

			gameObject->transform->Position += shakeOffset;
			at += shakeOffset;

			//if (shakeAmplitude < 0.01f) { shake = false; }
			//shakeCounter += Time::deltaTime;
			//time += Time::deltaTime;
			//if (shakeCounter >= 3.0f / FRAME_RATE) { power *= -1; shakeCounter = 0.0f; }
			//at.x += shakeValue * power * Time::fixedTimeScale;

			//if (time >= limit) { shake = false; cameraShakeBoolGlobal = false; }
		}
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
		ImGui::Text("Target : ");
		ImGui::SameLine();


		std::vector<std::string> list = Manager::GetScene()->GetGameObjectNames(GAMEOBJECT_LAYER);
		list.push_back("nullptr");

		if (Target == nullptr) { targetIndex = (int)list.size() - 1; }
		else { targetIndex = Target->GetID(); }
		if (ImGui::BeginCombo("##combo", list[targetIndex].c_str()))
		{
			for (int i = 0; i < list.size(); i++)
			{
				const bool isSelected = (targetIndex == i);

				if (ImGui::Selectable(list[i].c_str(), isSelected)) 
				{ 
					targetIndex = i;
					Target = Manager::GetScene()->Find(list[targetIndex].c_str());
				}
				if (isSelected) { ImGui::SetItemDefaultFocus(); }
			}

			ImGui::EndCombo();
		}

		ImGui::Text("Target : %s", (Target ? Target->GetTag().c_str() : "nullptr"));

		DebugManager::FloatDisplay(&fov, -FLT_MIN, "FOV", false, D3DXVECTOR2(1.0f, 120.0f), 1);

		DebugManager::Float3Display(&lightDirection, -1.0f, "Light", 0.01f, 2);


		if (ImGui::TreeNode("Details") && Target)
		{
			ImGui::Text("Target Transform");
			Target->transform->EngineDisplay();

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void Camera::SetTarget(GAMEOBJECT* value)
{
	Target = value;

	std::vector<std::string> list = Manager::GetScene()->GetGameObjectNames(GAMEOBJECT_LAYER);

	for (int i = 0; i < list.size(); i++)
	{
		if (Target->GetTag() == list[i])
		{
			targetIndex = i;
			return;
		}
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

D3DXVECTOR3 Camera::GetUp()
{
	D3DXMATRIX rot;
	D3DXMatrixInverse(&rot, nullptr, &mtxView);

	D3DXVECTOR3 up;
	up.x = rot._21;
	up.y = rot._22;
	up.z = rot._23;

	return up;
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

void Camera::CameraShake(D3DXVECTOR3 value, float f)
{
	shakeAmplitude = value;
	frequency = f;

	//if (cameraShakeBoolGlobal == false)
	//{
	//	cameraShakeBoolGlobal = true;
	//	shake = true;
	//	limit = t;
	//	shakeCounter = 0.0f;
	//	shakeValue = value;
	//	time = 0.0f;
	//}
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

template<class Archive>
void Camera::serialize(Archive & archive)
{
	std::string gameObjectName;
	if (Archive::is_saving::value)
	{
		if (Target != nullptr) { gameObjectName = Target->GetTag(); }
		else { gameObjectName = "nullptr"; }
	}

	archive(cereal::virtual_base_class<Component>(this),
		CEREAL_NVP(fov),
		cereal::make_nvp("Target", gameObjectName)
	);

	if (Archive::is_loading::value)
	{
		Target = Manager::GetScene()->Find(gameObjectName.c_str());
	}
}

template void Camera::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive& archive);
template void Camera::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive& archive);