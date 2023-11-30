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
	reflection = false;

	destroy = false;

	faceInDirection = false;
	quaternion = false;

	freezeX = false;
	freezeY = false;
	freezeZ = false;

	RingCounter = 0;

	tag = "";

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
		if (Time::timeScale > 0.0f)
		{
			if (material != nullptr)
			{
				material->Update();
			}

			for (auto com : components)
			{
				if (com->GetEnabled() == true)
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
				else
				{
					if (quaternion == true) { D3DXMatrixRotationQuaternion(&RotationMatrix, &transform->Quaternion); }
					else { D3DXMatrixRotationYawPitchRoll(&RotationMatrix, D3DXToRadian(Rotation.y), D3DXToRadian(Rotation.x), D3DXToRadian(Rotation.z)); }
				}
			}
			else if (faceInDirection == true) {}
			else 
			{
				if (quaternion == true) { D3DXMatrixRotationQuaternion(&RotationMatrix, &transform->Quaternion); }
				else { D3DXMatrixRotationYawPitchRoll(&RotationMatrix, D3DXToRadian(Rotation.y), D3DXToRadian(Rotation.x), D3DXToRadian(Rotation.z)); }
			}

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
				if (transform->boneMatrix != nullptr)
				{
					D3DXMATRIX temp;
					D3DXMATRIX convert = Mathf::MatrixConvert(*transform->boneMatrix);
					D3DXMatrixMultiply(&temp, &convert, &Parent->GetWorldMatrix());

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

void GAMEOBJECT::RemoveComponent(Component* com)
{
	com->End();
	components.remove(com);
	delete com;
}

void GAMEOBJECT::EngineDisplay()
{
	ImGui::Checkbox("", &active);
	ImGui::SameLine();
	ImGui::SeparatorText(tag.c_str());
	if (ImGui::TreeNode("GameObject"))
	{
		DebugManager::BoolDisplay(&depthShadow, -200.0f, "Shadow", 1);

		ImGui::Text("\nFreeze");
		DebugManager::BoolDisplay(&freezeX, -200.0f, "X", 2, true);
		ImGui::SameLine();
		DebugManager::BoolDisplay(&freezeY, -146.0f, "Y", 3, true);
		ImGui::SameLine();
		DebugManager::BoolDisplay(&freezeZ, -92.0f, "Z", 4, true);

		ImGui::Text("\n");
		ImGui::ColorPicker4("Color\n", Color);

		if (ImGui::TreeNode("Details"))
		{
			std::string n;
			std::string c;

			c = ((transform) ? "valid" : "nullptr");
			n = "Transform : " + c;
			ImGui::Text(n.c_str());

			c = ((rigidbody) ? "valid" : "nullptr");
			n = "Rigidbody : " + c;
			ImGui::Text(n.c_str());

			c = ((Parent) ? Parent->GetTag() : "nullptr");
			n = "Parent : " + c;
			ImGui::Text(n.c_str());

			ImGui::TreePop();
			ImGui::Spacing();
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

template<class Archive>
void GAMEOBJECT::serialize(Archive & archive)
{
	archive(CEREAL_NVP(active), CEREAL_NVP(Color));
	
	for (auto com : components)
	{
		if		(Transform* caster		= dynamic_cast<Transform*>(com))		{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (Camera* caster			= dynamic_cast<Camera*>(com))			{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (Number* caster			= dynamic_cast<Number*>(com))			{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (SphereCollider* caster = dynamic_cast<SphereCollider*>(com))	{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }

		//archive(cereal::make_nvp(commer->name.c_str(),*commer));
	}
}
CEREAL_CLASS_VERSION(GAMEOBJECT, 0);

template void GAMEOBJECT::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive& archive);
template void GAMEOBJECT::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive& archive);