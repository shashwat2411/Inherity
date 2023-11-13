#include "../main.h"
#include "../manager.h"
#include "../renderer.h"
#include "../MANAGEMENT FUNCTIONS/input.h"
#include "../OBJECT STORE/customScenes.h"
#include "../OBJECT STORE/component.h"
#include "../PRIVATE PATTERN/textureReader.h"
#include "../PRIVATE PATTERN/soundReader.h"
#include "../PRIVATE PATTERN/modelReader.h"
#include "../PRIVATE PATTERN/functions.h"
#include "../MANAGEMENT FUNCTIONS/debugManager.h"


SCENE* Manager::Scene{};	//static�����o�ϐ��͍Đ錾���K�v
SCENE* Manager::DontDestroyOnLoad{};	//static�����o�ϐ��͍Đ錾���K�v

Model ModelReader::ModelsOBJ[ModelReader::READ_MODEL_OBJ_MAX]{};
AnimationModel ModelReader::ModelsFBX[ModelReader::READ_MODEL_FBX_MAX]{};
std::unordered_map<std::string, const aiScene*> ModelReader::Animations{};

Audio SoundReader::Audios[SoundReader::READ_SOUND_MAX]{};

ID3D11ShaderResourceView* TextureReader::Textures[TextureReader::READ_TEXUTRE_MAX]{};

float Time::timeScale = 1.0f;
float Time::fixedTimeScale = 1.0f;
float Time::deltaTime = 1.0f / FRAME_RATE;

void LightInitialize(LIGHT* light, D3DXVECTOR3 position);

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();
	DebugManager::Init();

	SoundReader::ReadSound();
	ModelReader::ReadModel();
	TextureReader::ReadTexture();

	DontDestroyOnLoad = new EMPTY_SCENE();
	DontDestroyOnLoad->Init();

	SetScene<GAME_SCENE>();
	//SetScene<WORKSPACE_SCENE>();

	Time::timeScale = 1.0f;
	Time::fixedTimeScale = 1.0f;
	Time::deltaTime = 1.0f / FRAME_RATE;
}

void Manager::Uninit()
{

	Scene->Uninit();
	DontDestroyOnLoad->Uninit();

	//delete Scene;
	//delete DontDestroyOnLoad;


	ModelReader::UnReadModel();
	SoundReader::UnloadAudio();
	Audio::UninitMaster();
	TextureReader::UnReadTexture();

	Input::Uninit();

	DebugManager::Uninit();
	Renderer::Uninit();
}

void Manager::FixedUpdate()
{
	Input::Update();

	Scene->UpdateBefore();
	Scene->Update();

	DontDestroyOnLoad->UpdateBefore();
	DontDestroyOnLoad->Update();
}

void Manager::Draw()
{
	//���C�g�J�����\���̂̏�����
	LIGHT light;
	light.Enable = true;

	{
		//LightInitialize(&light, D3DXVECTOR3(-10.0f, 20.0f, -10.0f));
		//1�p�X��	�V���h�[�o�b�t�@�̍쐬
		Renderer::BeginDepth();
		Renderer::SetDepthViewPort();

		LightInitialize(&light, /*GetScene()->GetPlayer()->transform->Position*/D3DXVECTOR3(-10.0f, 0.0, 0.0f));
		//LightInitialize(&light, GetScene()->FindGameObject<PLAYERMODEL>()->transform->Position);

		//���C�g�J�����̍s����Z�b�g
		Renderer::SetLight(light);
		Renderer::SetViewMatrix(&light.viewMatrix);
		Renderer::SetProjectionMatrix(&light.projectionMatrix);

		//�e�𗎂Ƃ������I�u�W�F�N�g��`��i�ꉞ�n�ʂ��j
		Scene->DepthPath();
	}

	//3�p�X�ځ@�ʏ�̕`��
	Renderer::Begin();

	Renderer::SetDefaultViewPort();

	Scene->Draw();
	DontDestroyOnLoad->Draw();

	DebugManager::Draw();

	Renderer::End();

}

void Manager::Update()
{
	//Input::Update();

	//if (Input::GetKeyTrigger('V')) { Time::timeScale -= 0.1f; }
	//if (Input::GetKeyTrigger('B')) { Time::timeScale += 0.1f; }
}

void LightInitialize(LIGHT* light, D3DXVECTOR3 position)
{
	light->Direction = D3DXVECTOR4(1.0f, -1.0f, 0.0f, 0.0f);
	D3DXVec4Normalize(&light->Direction, &light->Direction);
	light->Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�J�����̃r���[�s����쐬
	D3DXVECTOR3 lightPos = D3DXVECTOR3(0.0f, 10.0f, -10.0f) + position;
	D3DXVECTOR3 lightTarget = position;
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&light->viewMatrix, &lightPos, &lightTarget, &lightUp);

	//���C�g�J�����̃v���W�F�N�V�����s����쐬
	D3DXMatrixPerspectiveFovLH(&light->projectionMatrix, 1.0, (float)(SCREEN_WIDTH) / (float)(SCREEN_HEIGHT), 5.0f, 50.0f);

}
