#include "../saveFunctions.h"
#include "manager.h"


void GAMEOBJECT::Initialize()
{
	active = true;
	billboard = false;
	collide = false;

	depth = true;
	depthShadow = false;
	reflection = false;
	geometry = false;

	destroy = false;

	faceInDirection = false;
	quaternion = false;

	freezeX = false;
	freezeY = false;
	freezeZ = false;

	id = 0;
	parentIndex = 0;
	RingCounter = 0;

	defaultY = 1.01f;
	cameraDistance = 0.0f;

	Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	transform = AddComponent<Transform>();
	shadow = nullptr;
	Parent = nullptr;
	rigidbody = nullptr;
	material = nullptr;

	VertexShader = nullptr;
	PixelShader = nullptr;
	VertexLayout = nullptr;

	transform->GlobalPosition = transform->Position;
}

void GAMEOBJECT::UnInitialize()
{
	if (material != nullptr) 
	{
		material->End();
		delete material; 
	}

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
	//cameraDistance = transform->DistanceFrom(Manager::GetScene()->GetCamera());

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

GAMEOBJECT* GAMEOBJECT::SetParent(GAMEOBJECT* parent) 
{ 
	Parent = parent; 
	Parent->Children.push_back(this);  
	
	std::vector<std::string> list = Manager::GetScene()->GetGameObjectNames(GAMEOBJECT_LAYER);

	for (int i = 0; i < list.size(); i++)
	{
		if (Parent->GetTag() == list[i])
		{
			parentIndex = i;
			return this;
		}
	}
	
	return this; 
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

			//c = ((Parent) ? Parent->GetTag() : "nullptr");
			//n = "Parent : " + c;
			//ImGui::Text(n.c_str());

			ImGui::Text("Parent: ");
			ImGui::SameLine();

			std::vector<std::string> list = Manager::GetScene()->GetGameObjectNames(GAMEOBJECT_LAYER);
			list.push_back("nullptr");

			if (Parent == nullptr) { parentIndex = (int)list.size() - 1; }
			else { parentIndex = Parent->GetID(); }
			if (ImGui::BeginCombo("##combo", list[parentIndex].c_str()))
			{
				for (int i = 0; i < list.size(); i++)
				{
					const bool isSelected = (parentIndex == i);

					if (ImGui::Selectable(list[i].c_str(), isSelected))
					{
						parentIndex = i;
						Parent = Manager::GetScene()->Find(list[parentIndex].c_str());
					}
					if (isSelected) { ImGui::SetItemDefaultFocus(); }
				}

				ImGui::EndCombo();
			}

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
	std::string gameObjectName;
	Material* mat = material;
	if (Archive::is_saving::value)
	{
		if (Parent != nullptr) { gameObjectName = Parent->GetTag(); }
		else { gameObjectName = "nullptr"; }
	}

	archive(CEREAL_NVP(active), CEREAL_NVP(Color), cereal::make_nvp("Parent", gameObjectName));
	
	for (auto com : components)
	{
		if		(Transform* caster		= dynamic_cast<Transform*>(com))		{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if	(Rigidbody* caster		= dynamic_cast<Rigidbody*>(com))		{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if	(SpriteRenderer* caster	= dynamic_cast<SpriteRenderer*>(com))	{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if	(Billboard* caster		= dynamic_cast<Billboard*>(com))		{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (Camera* caster			= dynamic_cast<Camera*>(com))			{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (Number* caster			= dynamic_cast<Number*>(com))			{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (SphereCollider* caster = dynamic_cast<SphereCollider*>(com))	{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (BoxCollider* caster	= dynamic_cast<BoxCollider*>(com))		{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (ParticleSystem* caster = dynamic_cast<ParticleSystem*>(com))	{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (AudioSource* caster	= dynamic_cast<AudioSource*>(com))		{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (MeshFilter* caster		= dynamic_cast<MeshFilter*>(com))		{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
		else if (ParticleSystem* caster	= dynamic_cast<ParticleSystem*>(com))	{ archive(cereal::make_nvp(caster->name.c_str(), *caster)); }
	}

	if (mat != nullptr) { archive(cereal::make_nvp("Material", *mat)); }

	if (Archive::is_loading::value)
	{
		Parent = Manager::GetScene()->Find(gameObjectName.c_str());

		if (mat != nullptr)
		{
			if (GetMaterial()->GetTexture("_Texture") != nullptr)
			{
				GetMaterial()->SetTexture("_Texture", ((TextureReader::READ_TEXTURE)*GetMaterial()->GetIndex()));
			}
		}
		
		AudioSource* as = GetComponent<AudioSource>();
		if (as != nullptr)
		{
			as->GetClip()->Stop();
			as->SetClip((SoundReader::READ_SOUND)as->GetSoundIndex());
		}

		MeshFilter* mf = GetComponent<MeshFilter>();
		if (mf != nullptr)
		{
			if (mf->GetModelIndex() < ModelReader::READ_MODEL_OBJ_MAX)
			{
				mf->SetModel((ModelReader::READ_MODEL_OBJ)mf->GetModelIndex());
			}
			else
			{
				mf->SetModel((ModelReader::READ_MODEL_FBX)(mf->GetModelIndex() - ModelReader::READ_MODEL_OBJ_MAX));
			}
		}
	}
}
CEREAL_CLASS_VERSION(GAMEOBJECT, 0);

template void GAMEOBJECT::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive& archive);
template void GAMEOBJECT::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive& archive);