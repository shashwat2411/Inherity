#include "customScenes.h"
#include "debugManager.h"
#include "postProcessManager.h"
#include "animations.h"


#define DEPTH_SHADOW_RENDERING
//#define ENVIRONMENT_MAPPING
//#define MIRROR_MAPPING

SCENE* Manager::Scene{};	//staticメンバ変数は再宣言が必要
SCENE* Manager::DontDestroyOnLoad{};	//staticメンバ変数は再宣言が必要

std::vector<Model> ModelReader::ModelsOBJ;
std::vector<AnimationModel> ModelReader::ModelsFBX;
std::unordered_map<std::string, const aiScene*> ModelReader::Animations;

std::array<Audio, SoundReader::READ_SOUND_MAX> SoundReader::Audios;

std::array<ID3D11ShaderResourceView*, TextureReader::READ_TEXTURE_MAX> TextureReader::Textures;

//Names
std::vector<const char*> ModelReader::modelNames;
std::array<const char*, SoundReader::READ_SOUND_MAX> SoundReader::soundNames;
std::array<const char*, TextureReader::READ_TEXTURE_MAX> TextureReader::textureNames;

float Time::timeScale = 1.0f;
float Time::fixedTimeScale = 1.0f;
float Time::deltaTime = 1.0f / FRAME_RATE;

void LightInitialize(LIGHT* light, D3DXVECTOR3 position);

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	DebugManager::Init();
	PostProcessManager::Init();
	Audio::InitMaster();
	
	SetScene<LOAD_SCENE>();
}

void Manager::Load()
{
	SoundReader::ReadSound();
	ModelReader::ReadModel();
	TextureReader::ReadTexture();

	DontDestroyOnLoad = new EMPTY_SCENE();
	DontDestroyOnLoad->Init();

	LOAD_SCENE::SetLoadOver(true);
}

void Manager::Unload()
{
	LOAD_SCENE::SetLoadOver(false);

	ModelReader::UnReadModel();
	SoundReader::UnloadAudio();
	TextureReader::UnReadTexture();
}

void Manager::Uninit()
{
	Scene->Uninit();
	if (DontDestroyOnLoad != nullptr) { DontDestroyOnLoad->Uninit(); }

	delete Scene;
	delete DontDestroyOnLoad;

	PostProcessManager::Uninit();
	Manager::Unload();

	Input::Uninit();
	DebugManager::Uninit();
	Renderer::Uninit();
}

void Manager::FixedUpdate()
{
	if (LOAD_SCENE::GetLogo() == false)
	{
		Input::Update();
		DebugManager::Update();
		PostProcessManager::Update();

		if (Time::timeScale > 0.0f)
		{
			COLLISION::Update();

			Scene->UpdateBefore();
			if (DontDestroyOnLoad != nullptr) { DontDestroyOnLoad->UpdateBefore(); }

			Scene->Update();
			if (DontDestroyOnLoad != nullptr) { DontDestroyOnLoad->Update(); }
		}

		if (Input::GetKeyPress(VK_CONTROL))
		{
			if (Input::GetKeyTrigger('S')) { Save(Scene->name); }
			if (Input::GetKeyTrigger('O')) { Open(Scene->name); }
		}
	}
	else
	{
		DebugManager::Update();
		PostProcessManager::Update();

		Scene->UpdateBefore();
		Scene->Update();
	}
}

void Manager::Draw()
{
	if (LOAD_SCENE::GetLogo() == false)
	{
		LIGHT light;
		light.Enable = true;

#ifdef DEPTH_SHADOW_RENDERING
		//1パス目	シャドーバッファの作成
		{
			Renderer::BeginDepth();
			Renderer::SetDepthViewPort();

			if (GetScene()->GetPlayer() != nullptr)
			{
				LightInitialize(&light, GetScene()->GetPlayer()->transform->Position);
			}

			//ライトカメラの行列をセット
			Renderer::SetLight(light);
			Renderer::SetProjectionMatrix(&light.projectionMatrix);
			Renderer::SetViewMatrix(&light.viewMatrix);

			//影を落としたいオブジェクトを描画
			Scene->DepthPath();
		}
#endif

#ifdef ENVIRONMENT_MAPPING
		//2パス目　環境マップイング
		{
			D3DXMATRIX viewMatrixArray[6];
			D3DXMATRIX projectionMatrix;

			Scene->ReflectionMap(&viewMatrixArray[0]);

			D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 2, 1.0f, 0.01f, 120.0f);
			Renderer::SetProjectionMatrix(&projectionMatrix);

			//ビューポート変更
			Renderer::SetReflectViewPort();

			//6面分描画する
			for (int i = 0; i < 6; i++)
			{
				Renderer::BeginCube();

				//ビュー変換 Matrix 設定 
				Renderer::SetViewMatrix(&viewMatrixArray[i]);

				Scene->EnvironmentMap();

				Renderer::GetDeviceContext()->CopySubresourceRegion(Renderer::GetCubeReflectTexture(), D3D11CalcSubresource(0, i, 1), 0, 0, 0, Renderer::GetReflectTexture(), 0, nullptr);
			}

		}
#endif

#ifdef MIRROR_MAPPING
		//3パス目　鏡の環境マッピング
		{
			D3DXVECTOR3 lookatOffset = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	//+Y D3D11_TEXTURECUBE_FACE_POSITIVE_Y
			D3DXVECTOR3 upOffset = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			D3DXVECTOR3 eye;
			D3DXVECTOR3 lookAt;
			D3DXVECTOR3 up;

			D3DXMATRIX view;
			D3DXMATRIX projectionMatrix;

			//D3DXVECTOR3 vPlayerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//if (GetScene()->GetPlayer() != nullptr)
			//{
				D3DXVECTOR3 vPlayerPos = GetScene()->GetReflector()->transform->GlobalPosition;
			//}

			eye = vPlayerPos;
			lookAt = vPlayerPos + lookatOffset;
			up = upOffset;	
			D3DXMatrixLookAtLH(&view, &eye, &lookAt, &up);

			D3DXMatrixPerspectiveFovLH(&projectionMatrix, 120.0f, 1.0f, 0.01f, 120.0f);
			Renderer::SetProjectionMatrix(&projectionMatrix);

			Renderer::SetMirrorViewPort();
			Renderer::BeginMirror();

			//ビュー変換 Matrix 設定 
			Renderer::SetViewMatrix(&view);

			Scene->EnvironmentMap();
		}
#endif

		//4パス目　通常の描画
		{
			Renderer::BeginPostProcess();
			Renderer::SetDefaultViewPort();

			Scene->Draw();
			if (DontDestroyOnLoad != nullptr && Time::timeScale > 0.0f) { DontDestroyOnLoad->Draw(); }

			PostProcessManager::Draw();

			DebugManager::DebugDraw(Scene);
			DebugManager::Draw();

			Renderer::End();
		}
	}
	else
	{
		Renderer::Begin();
		Renderer::SetDefaultViewPort();

		Scene->Draw();

		DebugManager::Draw();
		Renderer::End();
	}
}

void Manager::Update()
{

}

void LightInitialize(LIGHT* light, D3DXVECTOR3 position)
{
	//Spot Light
	light->Direction = D3DXVECTOR4(1.0f, -1.0f, 0.0f, 0.0f);
	D3DXVec4Normalize(&light->Direction, &light->Direction);
	light->Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトカメラのビュー行列を作成
	D3DXVECTOR3 lightPos = D3DXVECTOR3(-10.0f, 30.0f, 0.0f) + position;
	D3DXVECTOR3 lightTarget = position;
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light->viewMatrix, &lightPos, &lightTarget, &lightUp);

	//ライトカメラのプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light->projectionMatrix, 1.0f, (float)(SCREEN_WIDTH) / (float)(SCREEN_WIDTH), 5.0f, 100.0f);

}

void Manager::Save(std::string name)
{
	std::string fileName = name + ".txt";
	std::ofstream outFile(fileName.c_str());

	if (!outFile.is_open()) { return; }

	cereal::JSONOutputArchive archive(outFile);

	archive(CEREAL_NVP(GetScene()->objectAdder));
	archive(CEREAL_NVP(GetScene()->componentAdder));
	archive(CEREAL_NVP(GetScene()->animationAdder));

	for (int i = 0; i < MAX_LAYER; i++)
	{
		for (GAMEOBJECT* object : Scene->GetGameObjectList((LAYER)i))
		{
			archive(cereal::make_nvp(object->GetTag(), *object));
		}
	}
}

void Manager::Open(std::string name)
{
	std::string fileName = name + ".txt";
	std::ifstream inFile(fileName.c_str());

	if (!inFile.is_open()) { return; }

	cereal::JSONInputArchive archive(inFile);

	std::vector<AddObjectSaveFile> adder;
	archive(adder);
	for(AddObjectSaveFile add : adder)
	{
		if (add.name == "CUBE")				{ for (int i = 0; i < add.number; i++) { GetScene()->AddGameObject<CUBE>(ObjectIndex("Cube(Clone)")); } }
		if (add.name == "CYLINDER")			{ for (int i = 0; i < add.number; i++) { GetScene()->AddGameObject<CYLINDER>(ObjectIndex("Cylinder(Clone)")); } }
		if (add.name == "IMAGE")			{ for (int i = 0; i < add.number; i++) { GetScene()->AddGameObject<IMAGE>(ObjectIndex("Sprite(Clone)"), SPRITE_LAYER); } }
		if (add.name == "BILLBOARD")		{ for (int i = 0; i < add.number; i++) { GetScene()->AddGameObject<BILLBOARD>(ObjectIndex("Billboard(Clone)"), BILLBOARD_LAYER); } }
		if (add.name == "PARTICLESYSTEM")	{ for (int i = 0; i < add.number; i++) { GetScene()->AddGameObject<PARTICLESYSTEM>(ObjectIndex("ParticleSystem(Clone)"), BILLBOARD_LAYER); } }
		if (add.name == "EMPTYOBJECT")		{ for (int i = 0; i < add.number; i++) { GetScene()->AddGameObject<EMPTYOBJECT>(ObjectIndex("EmptyObject(Clone)")); } }
		if (add.name == "ENEMY")			{ for (int i = 0; i < add.number; i++) { GetScene()->AddGameObject<ENEMY>(ObjectIndex("Enemy(Clone)")); } }
	}
	GetScene()->objectAdder = adder;

	std::vector<AddComponentSaveFile> cdder;
	archive(cdder);
	for (AddComponentSaveFile cdd : cdder)
	{
		if (cdd.name == "Animator")			{ GetScene()->Find(cdd.gameObject.c_str())->AddComponent<Animator>(); }
		if (cdd.name == "AudioListener")	{ GetScene()->Find(cdd.gameObject.c_str())->AddComponent<AudioListener>(); }
		if (cdd.name == "AudioSource")		{ GetScene()->Find(cdd.gameObject.c_str())->AddComponent<AudioSource>(); }
		if (cdd.name == "Rigidbody")		{ GetScene()->Find(cdd.gameObject.c_str())->AddComponent<Rigidbody>(); }
		if (cdd.name == "SphereCollider")	{ GetScene()->Find(cdd.gameObject.c_str())->AddComponent<SphereCollider>(); }
		if (cdd.name == "BoxCollider")		{ GetScene()->Find(cdd.gameObject.c_str())->AddComponent<BoxCollider>(); }
		if (cdd.name == "MeshFilter")		{ GetScene()->Find(cdd.gameObject.c_str())->AddComponent<MeshFilter>(); }
		if (cdd.name == "ParticleSystem")	{ GetScene()->Find(cdd.gameObject.c_str())->AddComponent<ParticleSystem>(); }
	}
	GetScene()->componentAdder = cdder;

	std::vector<AddComponentSaveFile> animadder;
	archive(animadder);
	for (AddComponentSaveFile animdd : animadder)
	{
		if (animdd.name == "CharacterRetract")		{ GetScene()->Find(animdd.gameObject.c_str())->GetComponent<Animator>()->AddAnimation<CharacterRetract>(); }
		if (animdd.name == "TrialAnimation")		{ GetScene()->Find(animdd.gameObject.c_str())->GetComponent<Animator>()->AddAnimation<TrialAnimation>(); }
		if (animdd.name == "TrialAnimation2")		{ GetScene()->Find(animdd.gameObject.c_str())->GetComponent<Animator>()->AddAnimation<TrialAnimation2>(); }
		if (animdd.name == "TitleDomeAnimation")	{ GetScene()->Find(animdd.gameObject.c_str())->GetComponent<Animator>()->AddAnimation<TitleDomeAnimation>(); }
	}
	GetScene()->animationAdder = animadder;

	for (int i = 0; i < MAX_LAYER; i++)
	{
		for (GAMEOBJECT* object : Scene->GetGameObjectList((LAYER)i))
		{
			archive(*object);
		}
	}

	for (int i = 0; i < MAX_LAYER; i++)
	{
		for (GAMEOBJECT* object : GetScene()->GetGameObjectList((LAYER)i))
		{
			for (Component* com : object->GetComponentList())
			{
				if (SpriteRenderer* caster = dynamic_cast<SpriteRenderer*>(com))
				{
					object->GetMaterial()->SetTexture("_Texture", ((TextureReader::READ_TEXTURE)*object->GetMaterial()->GetIndex()));
				}
			}
		}
	}
}

std::string ObjectIndex(std::string name)
{
	std::string str;

	int num = (int)Manager::GetScene()->FindMultiple(name).size();
	str = name + (num > 0 ? "_" + std::to_string(num) : "");

	return str;
}