#include "main.h"
#include "renderer.h"
#include "customScenes.h"
#include "textureReader.h"
#include "soundReader.h"
#include "modelReader.h"
#include "debugManager.h"
#include "postProcess.h"


POSTPROCESS* Manager::PostProcess{};
SCENE* Manager::Scene{};	//staticメンバ変数は再宣言が必要
SCENE* Manager::DontDestroyOnLoad{};	//staticメンバ変数は再宣言が必要

Model ModelReader::ModelsOBJ[ModelReader::READ_MODEL_OBJ_MAX]{};
AnimationModel ModelReader::ModelsFBX[ModelReader::READ_MODEL_FBX_MAX]{};
std::unordered_map<std::string, const aiScene*> ModelReader::Animations{};

Audio SoundReader::Audios[SoundReader::READ_SOUND_MAX]{};

ID3D11ShaderResourceView* TextureReader::Textures[TextureReader::READ_TEXTURE_MAX]{};

float Time::timeScale = 1.0f;
float Time::fixedTimeScale = 1.0f;
float Time::deltaTime = 1.0f / FRAME_RATE;

void LightInitialize(LIGHT* light, D3DXVECTOR3 position);

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	DebugManager::Init();

	SoundReader::ReadSound();
	ModelReader::ReadModel();
	TextureReader::ReadTexture();

	DontDestroyOnLoad = new EMPTY_SCENE();
	DontDestroyOnLoad->Init();

	//PostProcess = new POSTPROCESS();
	//PostProcess->Init();

	SetScene<GAME_SCENE>();
	//SetScene<WORKSPACE_SCENE>();

	Time::timeScale = 0.0f;
	Time::fixedTimeScale = 1.0f;
	Time::deltaTime = 1.0f / FRAME_RATE;
}

void Manager::Uninit()
{
	if (PostProcess) { PostProcess->Uninit(); }

	Scene->Uninit();
	DontDestroyOnLoad->Uninit();

	delete Scene;
	delete DontDestroyOnLoad;


	ModelReader::UnReadModel();
	SoundReader::UnloadAudio();
	TextureReader::UnReadTexture();

	Input::Uninit();

	DebugManager::Uninit();
	Renderer::Uninit();
}

void Manager::FixedUpdate()
{
	Input::Update();
	DebugManager::Update();

	if (Time::timeScale > 0.0f)
	{
		Scene->UpdateBefore();
		Scene->Update();

		DontDestroyOnLoad->UpdateBefore();
		DontDestroyOnLoad->Update();

		if (PostProcess) { PostProcess->Update(); }
	}

	COLLISION::Update();
}

void Manager::Draw()
{
	//ライトカメラ構造体の初期化
	LIGHT light;
	light.Enable = true;

	//1パス目	シャドーバッファの作成
	{
		Renderer::BeginDepth();
		Renderer::SetDepthViewPort();

		if (GetScene()->GetPlayer() != nullptr)
		{
			LightInitialize(&light, GetScene()->GetPlayer()->transform->Position/*D3DXVECTOR3(-10.0f, 0.0, 0.0f)*/);
		}

		//ライトカメラの行列をセット
		Renderer::SetLight(light);
		Renderer::SetProjectionMatrix(&light.projectionMatrix);
		Renderer::SetViewMatrix(&light.viewMatrix);

		//影を落としたいオブジェクトを描画（一応地面も）
		Scene->DepthPath();
	}

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

	//3パス目　通常の描画
	{
		if (PostProcess) { Renderer::BeginPostProcess(); }
		else { Renderer::Begin(); }

		Renderer::SetDefaultViewPort();


		Scene->Draw();
		DontDestroyOnLoad->Draw();

		if (PostProcess) { Renderer::Begin(); PostProcess->Draw(); }

		DebugManager::DebugDraw(Scene);
		DebugManager::Draw();

		Renderer::End();
	}
}

void Manager::Update()
{
	//Input::Update();

	//if (Input::GetKeyTrigger('V')) { Time::timeScale -= 0.1f; }
	//if (Input::GetKeyTrigger('B')) { Time::timeScale += 0.1f; }
}

void LightInitialize(LIGHT* light, D3DXVECTOR3 position)
{
	//Spot Light
	light->Direction = D3DXVECTOR4(1.0f, -1.0f, 0.0f, 0.0f);
	D3DXVec4Normalize(&light->Direction, &light->Direction);
	light->Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ライトカメラのビュー行列を作成
	D3DXVECTOR3 lightPos = D3DXVECTOR3(-10.0f, 10.0f, 0.0f) + position;
	D3DXVECTOR3 lightTarget = position;
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light->viewMatrix, &lightPos, &lightTarget, &lightUp);

	//ライトカメラのプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light->projectionMatrix, 1.0f, (float)(SCREEN_WIDTH) / (float)(SCREEN_HEIGHT), 5.0f, 20.0f);

}
