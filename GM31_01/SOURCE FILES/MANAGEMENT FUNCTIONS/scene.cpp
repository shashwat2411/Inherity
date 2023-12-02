#include "scene.h"
#include "manager.h"
#include "soundReader.h"

void SCENE::InitBefore()
{
	end = false;

	name = "";

	//GAMEOBJECT
	MainCamera = AddGameObject<CAMERA>("MainCamera", CAMERA_LAYER);
	reflectionProjector = AddGameObject<EMPTYOBJECT>("Reflection");

	//UI
	Fade = Manager::GetDontDestroyOnLoadScene()->FindGameObject<FADE>();
	if (Fade == nullptr) { Fade = Manager::GetDontDestroyOnLoadScene()->AddGameObject<FADE>("Fade", FADE_LAYER); }

	reflectionProjector->transform->Position = D3DXVECTOR3(0.0f, 3.0f, 0.0f);

	Fade->SetFadeIn();

}

void SCENE::Uninit()
{
	for (int i = 0; i < MAX_LAYER; i++)
	{
		for (auto var : GameObjects[i])
		{
			var->UnInitialize();
			delete var;
		}

		GameObjects[i].clear();
	}
	SoundReader::StopAllSound();
}

void SCENE::UpdateBefore()
{
	for (int i = 0; i < MAX_LAYER; i++)
	{
		if (i != GIZMO_LAYER || (DebugManager::gizmo == true && DebugManager::play == true))
		{
			for (auto var : GameObjects[i])
			{
				if (var->GetActive() == true)
				{
					var->Update();
				}
			}
			GameObjects[i].remove_if([](GAMEOBJECT* object) {return object->Remove(); });
		}
	}
	COLLISION::Update();
}

void SCENE::Draw()
{
	std::sort(gameObjectNames.begin(), gameObjectNames.end());

	for (int i = 0; i < MAX_LAYER; i++)
	{
		if (i != GIZMO_LAYER || (DebugManager::gizmo == true && DebugManager::play == false))
		{
			for (auto var : GameObjects[i])
			{
				var->Draw();
			}
		}
	}
}

void SCENE::FakeDraw(GAMEOBJECT* var)
{
	MeshFilter* mesh = var->GetComponent<MeshFilter>();
	MeshField* field = var->GetComponent<MeshField>();
	Plane* plane = var->GetComponent<Plane>();
	Billboard* billboard = var->GetComponent<Billboard>();
	if (mesh != nullptr)
	{
		//入力レイアウト設定
		Renderer::GetDeviceContext()->IASetInputLayout(var->GetVertexLayout());

		//シェーダー設定
		Renderer::GetDeviceContext()->VSSetShader(var->GetVertexShader(), NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(var->GetPixelShader(), NULL, 0);

		D3DXVECTOR3 Position = var->transform->Position;
		D3DXVECTOR3 Rotation = var->transform->Rotation;
		D3DXVECTOR3 Scale = var->transform->Scale;

		//マトリクス設定
		D3DXMatrixScaling(&var->GetScaleMatrix(), Scale.x, Scale.y, Scale.z);

		if (var->rigidbody != nullptr)
		{
			if (var->rigidbody->FollowTarget != nullptr) {}
			else { D3DXMatrixRotationYawPitchRoll(&var->GetRotationMatrix(), D3DXToRadian(Rotation.y), D3DXToRadian(Rotation.x), D3DXToRadian(Rotation.z)); }
		}
		else if (var->GetFaceInDirection() == true) {}
		else { D3DXMatrixRotationYawPitchRoll(&var->GetRotationMatrix(), D3DXToRadian(Rotation.y), D3DXToRadian(Rotation.x), D3DXToRadian(Rotation.z)); }


		D3DXMatrixTranslation(&var->GetTransformMatrix(), Position.x, Position.y, Position.z);

		D3DXMatrixMultiply(&var->GetWorldMatrix(var->GetRingCounter()), &var->GetScaleMatrix(), &var->GetRotationMatrix());
		D3DXMatrixMultiply(&var->GetWorldMatrix(var->GetRingCounter()), &var->GetWorldMatrix(var->GetRingCounter()), &var->GetTransformMatrix()); //World = World * Translation

		if (var->Parent != nullptr)
		{
			D3DXVec3TransformCoord(&var->transform->GlobalPosition, &var->transform->Position, &var->Parent->GetWorldMatrix()); //Global Position
			D3DXMatrixMultiply(&var->GetWorldMatrix(var->GetRingCounter()), &var->GetWorldMatrix(var->GetRingCounter()), &var->Parent->GetWorldMatrix()); //World = World * Parent->World
		}
		else { var->transform->GlobalPosition = var->transform->Position; }

		Renderer::SetWorldMatrix(&var->GetWorldMatrix(0));


		if (var->GetMaterial() != nullptr)
		{
			var->GetMaterial()->Draw();
		}

		mesh->Draw();
	}
	else if (field != nullptr) { field->Draw(); }
	else if (plane != nullptr) { plane->Draw(); }
	else if (billboard != nullptr) { billboard->Draw(); }
}

void SCENE::DepthPath()
{
	for (int i = 0; i < MAX_LAYER; i++)
	{
		for (auto var : GameObjects[i])
		{
			if (var->GetDepthShadow() == true)
			{
				FakeDraw(var);
			}
		}
	}
}

void SCENE::EnvironmentMap()
{
	for (int i = 0; i < MAX_LAYER; i++)
	{
		for (auto var : GameObjects[i])
		{
			if (var->GetReflection() == true)
			{
				FakeDraw(var);
			}
		}
	}
}

void SCENE::ReflectionMap(D3DXMATRIX* view)
{
	D3DXVECTOR3 lookatOffset[6] = {
	{  1.0f,  0.0f,  0.0f },	//+X D3D11_TEXTURECUBE_FACE_POSITIVE_X
	{ -1.0f,  0.0f,  0.0f },	//-X D3D11_TEXTURECUBE_FACE_NEGATIVE_X
	{  0.0f,  1.0f,  0.0f },	//+Y D3D11_TEXTURECUBE_FACE_POSITIVE_Y
	{  0.0f, -1.0f,  0.0f },	//-Y D3D11_TEXTURECUBE_FACE_NEGATIVE_Y
	{  0.0f,  0.0f,  1.0f },	//+Z D3D11_TEXTURECUBE_FACE_POSITIVE_Z
	{  0.0f,  0.0f, -1.0f },	//-Z D3D11_TEXTURECUBE_FACE_NEGATIVE_Z
	};

	//注視点オフセットテーブル
	D3DXVECTOR3 upOffset[6] = {
		{  0.0f,  1.0f,  0.0f },	//+X D3D11_TEXTURECUBE_FACE_POSITIVE_X
		{  0.0f,  1.0f,  0.0f },	//-X D3D11_TEXTURECUBE_FACE_NEGATIVE_X
		{  0.0f,  0.0f, -1.0f },	//+Y D3D11_TEXTURECUBE_FACE_POSITIVE_Y
		{  0.0f,  0.0f,  1.0f },	//-Y D3D11_TEXTURECUBE_FACE_NEGATIVE_Y
		{  0.0f,  1.0f,  0.0f },	//+Z D3D11_TEXTURECUBE_FACE_POSITIVE_Z
		{  0.0f,  1.0f,  0.0f },	//-Z D3D11_TEXTURECUBE_FACE_NEGATIVE_Z
	};

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 up;


	D3DXVECTOR3 vPlayerPos = reflectionProjector->transform->GlobalPosition;
	for (int j = 0; j < 6; j++)
	{
		eye = vPlayerPos;
		lookAt = vPlayerPos + lookatOffset[j];
		up = upOffset[j];
		D3DXMatrixLookAtLH(&view[j], &eye, &lookAt, &up);
	}
}
