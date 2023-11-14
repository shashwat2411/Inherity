#include "gameobject.h"
#include "component.h"
#include "material.h"
#include "manager.h"
#include "material.h"

void GAMEOBJECT::Initialize()
{
	active = true;
	billboard = false;
	collide = false;

	depth = true;
	depthShadow = false;

	destroy = false;
	ignorePause = false;

	faceInDirection = false;
	parentMatrixEnable = false;

	freezeX = false;
	freezeY = false;
	freezeZ = false;

	RingCounter = 0;

	tag[64];

	defaultY = 1.01f;
	cameraDistance = 0.0f;

	Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	transform = AddComponent<Transform>();
	shadow = nullptr;
	Parent = nullptr;
	rigidbody = nullptr;

	VertexShader = nullptr;
	PixelShader = nullptr;
	VertexLayout = nullptr;


	if (Parent != nullptr) { D3DXVec3TransformCoord(&transform->GlobalPosition, &transform->Position, &Parent->GetWorldMatrix()); }
	else { transform->GlobalPosition = transform->Position; }
}

void GAMEOBJECT::UnInitialize()
{
	delete material;

	for (auto com : components)
	{
		com->End();
		delete com;
	}

	if (VertexLayout != nullptr) { VertexLayout->Release(); }
	if (VertexShader != nullptr) { VertexShader->Release(); }
	if (PixelShader  != nullptr) { PixelShader->Release();  }
}

void GAMEOBJECT::Draw()
{
	cameraDistance = transform->DistanceFrom(Manager::GetScene()->GetCamera());

	if (active == true)
	{
		if (material != nullptr)
		{
			material->Update();
		}

		for (auto com : components)
		{
			if (com->GetEnabled() == true)
			{
				if (Manager::GetScene()->GetPaused() == false || com->GetIgnorePause() == true)
				{
					com->Update();
				}
			}
		}

		if (depth == true)
		{
			//入力レイアウト設定
			Renderer::GetDeviceContext()->IASetInputLayout(VertexLayout);

			//シェーダー設定
			Renderer::GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
			Renderer::GetDeviceContext()->PSSetShader(PixelShader, NULL, 0);

			D3DXVECTOR3 Position = transform->Position;
			D3DXVECTOR3 Rotation = transform->Rotation;
			D3DXVECTOR3 Scale = transform->Scale;

			//マトリクス設定
			D3DXMatrixScaling(&ScaleMatrix, Scale.x, Scale.y, Scale.z);

			if (rigidbody != nullptr)
			{
				if (rigidbody->FollowTarget != nullptr) {}
				else { D3DXMatrixRotationYawPitchRoll(&RotationMatrix, D3DXToRadian(Rotation.y), D3DXToRadian(Rotation.x), D3DXToRadian(Rotation.z)); }
			}
			else if (faceInDirection == true) {}
			else { D3DXMatrixRotationYawPitchRoll(&RotationMatrix, D3DXToRadian(Rotation.y), D3DXToRadian(Rotation.x), D3DXToRadian(Rotation.z)); }

			D3DXMATRIX view = Manager::GetScene()->GetCamera()->GetComponent<Camera>()->GetViewMatrix();
			D3DXMATRIX invView;
			D3DXMatrixInverse(&invView, NULL, &view);
			invView._41 = 0.0f;
			invView._42 = 0.0f;
			invView._43 = 0.0f;

			D3DXMatrixTranslation(&TransformMatrix, Position.x, Position.y, Position.z);

			if (billboard == true) { D3DXMatrixMultiply(&WorldMatrix[RingCounter], &ScaleMatrix, /*&RotationMatrix*/&invView); }  //World = Scaling * Rotation
			else { D3DXMatrixMultiply(&WorldMatrix[RingCounter], &ScaleMatrix, &RotationMatrix); }

			D3DXMatrixMultiply(&WorldMatrix[RingCounter], &WorldMatrix[RingCounter], &TransformMatrix); //World = World * Translation

			if (Parent != nullptr)
			{
				if (parentMatrixEnable == true)
				{
					D3DXMATRIX temp;
					D3DXMatrixMultiply(&temp, &parentMatrix, &Parent->GetWorldMatrix());

					D3DXVec3TransformCoord(&transform->GlobalPosition, &transform->Position, &temp); //Global Position
					D3DXMatrixMultiply(&WorldMatrix[RingCounter], &WorldMatrix[RingCounter], &temp); //World = World * Parent->World
				}
				else
				{
					D3DXVec3TransformCoord(&transform->GlobalPosition, &transform->Position, &Parent->GetWorldMatrix()); //Global Position
					D3DXMatrixMultiply(&WorldMatrix[RingCounter], &WorldMatrix[RingCounter], &Parent->GetWorldMatrix()); //World = World * Parent->World
				}
			}
			else { transform->GlobalPosition = transform->Position; }

			//if (parentMatrixEnable == true)
			//{
			//	D3DXVECTOR3 globalScale;
			//	D3DXVec3TransformCoord(&transform->GlobalPosition, &transform->GlobalPosition, &parentMatrix); //Global Position
			//	D3DXVec3TransformCoord(&globalScale, &transform->Scale, &parentMatrix); //Global Position
			//	D3DXMatrixMultiply(&WorldMatrix[RingCounter], &WorldMatrix[RingCounter], &parentMatrix); //World = World * Parent->World
			//}
			//else if (Parent == nullptr) { transform->GlobalPosition = transform->Position; }

			if (GetComponent<Afterimage>() != nullptr)
			{
				for (int i = RingCounter, n = 0; i < RingCounter + GetComponent<Afterimage>()->imageNum; i++, n++)
				{
					int index = RingCounter - (n * 16);
					if (index < 0) { index += MAX_AFTERIMAGES; }

					Renderer::SetWorldMatrix(&WorldMatrix[index]);
					if (GetComponent<MeshFilter>() != nullptr)
					{
						GetComponent<MeshFilter>()->Draw();
					}
					//for (auto child : Children)
					//{
					//	if (child->GetComponent<MeshFilter>() != nullptr)
					//	{
					//		if (child->GetComponent<MeshFilter>()->GetCollider() == false)
					//		{
					//			//child->GetComponent<MeshFilter>()->Draw();
					//		}
					//	}
					//}
				}
			}
			else
			{
				Renderer::SetWorldMatrix(&WorldMatrix[0]);
			}

		}

		if (material != nullptr)
		{
			material->Draw();
		}

		for (auto com : components)
		{
			if (com->GetEnabled() == true)
			{
				com->Draw();
			}
		}
	}
}

void GAMEOBJECT::Destroy(bool value)
{
	destroy = value;
	for (auto child : Children) { child->Destroy(value); }
	if (shadow != nullptr) { shadow->shadow->Destroy(); }
}

bool GAMEOBJECT::Remove()
{
	if (destroy == true)
	{
		if (rigidbody != nullptr)
		{
			this->RemoveComponent<Rigidbody>();
		}
		UnInitialize();
		delete this;
		return true;
	}
	else
	{
		return false;
	}
}
