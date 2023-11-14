#include "scene.h"
#include "manager.h"
#include "soundReader.h"

void SCENE::InitBefore()
{
	end = false;
	paused = false;

	Collision.Init();


	//GAMEOBJECT
	MainCamera = AddGameObject<CAMERA>(CAMERA_LAYER);

	//UI
	Fade = Manager::GetDontDestroyOnLoadScene()->FindGameObject<FADE>();
	if (Fade == nullptr) { Fade = Manager::GetDontDestroyOnLoadScene()->AddGameObject<FADE>(FADE_LAYER); }

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
		for (auto var : GameObjects[i])
		{
			if (paused == false || var->GetIgnorePause() == true)
			{
				if (var->GetActive() == true)
				{
					var->Update();
				}
			}
		}
		GameObjects[i].remove_if([](GAMEOBJECT* object) {return object->Remove(); });
	}
	if (paused == false) { Collision.Update(); }

}

void SCENE::Draw()
{
	for (int i = 0; i < MAX_LAYER; i++)
	{
		for (auto var : GameObjects[i])
		{
			var->Draw();
		}
	}
}

void SCENE::DepthPath()
{
	for (int i = 0; i < MAX_LAYER; i++)
	{
		for (auto var : GameObjects[i])
		{
			if (var->GetDepthShadow() == true)
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
					//if (mesh->GetFBX() == true) 
					//{ 
					//	mesh->GetModel()->Draw(); 
					//}
					//else 
					//{ 
					//	mesh->GetModel(0)->Draw(); 
					//}
				}
				else if (field != nullptr) { field->Draw(); }
				else if (plane != nullptr) { plane->Draw(); }
				else if (billboard != nullptr) { billboard->Draw(); }
			}
		}
	}
}