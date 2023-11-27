#include "debugManager.h"
#include "main.h"
#include "renderer.h"
#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_win32.h"
#include "../imGUI/imgui_impl_dx11.h"
#include "manager.h"
#include "input.h"


bool play = false;
bool paused = false;
bool show_demo_window = true;

int index = 0;
int layer = 0;

float speed = 0.1f;

const char* str[MAX_LAYER] =
{
	"CAMERA_LAYER",
	"GAMEOBJECT_LAYER",
	"COLLIDER_LAYER",
	"GIZMO_LAYER",
	"BILLBOARD_LAYER",
	"SHADOW_LAYER",
	"SPRITE_LAYER",
	"FADE_LAYER"
};

EDIT_MODE edit = EDIT_MODE::POSITION;

void DebugManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());


	io.Fonts->AddFontDefault();
}

void DebugManager::Uninit()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugManager::Update()
{
	// フレームの開始
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DebugManager::Draw()
{
	 //1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::EndFrame();
	// ImGuiコード
	//フレームの描画
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DebugManager::DebugDraw(SCENE * scene)
{
	std::vector<GAMEOBJECT*> vector = scene->GetGameObjectListVector((LAYER)layer);

	//ImGui::SetNextWindowSize(ImVec2(140, 75));

	ImGui::Begin("Index", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGui::PushItemWidth(-FLT_MIN);
	ImGui::SliderInt(" ", &layer, 0, MAX_LAYER - 1, str[layer]);

	if (index > vector.size() - 1) { index = vector.size() - 1; }
	ImGui::PushItemWidth(-FLT_MIN);
	if (vector[0] != nullptr) { ImGui::SliderInt("", &index, 0, vector.size() - 1, vector[index]->GetTag().c_str()); }

	ImGui::End();

	//ImGui::SetNextWindowSize(ImVec2(200, 170));
	ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		if (vector[0] != nullptr)
		{
			if (play == false || paused == true)
			{
				if (Input::GetKeyTrigger('1')) { edit = EDIT_MODE::POSITION; }
				if (Input::GetKeyTrigger('2')) { edit = EDIT_MODE::ROTATION; }
				if (Input::GetKeyTrigger('3')) { edit = EDIT_MODE::SCALE; }

				GAMEOBJECT* mover = vector[index];
				if (Input::GetKeyPress(VK_LSHIFT)) { mover = scene->GetCamera(); }

				D3DXVECTOR3 directionX(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 directionY(0.0f, 0.0f, 0.0f);
				D3DXVECTOR3 directionZ(0.0f, 0.0f, 0.0f);

				if (edit != EDIT_MODE::ROTATION)
				{
						 if (Input::GetKeyPress('W')) { /*mover->transform->Position.z += 0.1f;*/ directionZ =  scene->GetCamera()->camera->GetForward();	}
					else if (Input::GetKeyPress('S')) { /*mover->transform->Position.z -= 0.1f;*/ directionZ = -scene->GetCamera()->camera->GetForward();	}
						 if (Input::GetKeyPress('Q')) { /*mover->transform->Position.y += 0.1f;*/ directionY =  scene->GetCamera()->camera->GetUp();		}
					else if (Input::GetKeyPress('E')) { /*mover->transform->Position.y -= 0.1f;*/ directionY = -scene->GetCamera()->camera->GetUp();		}
						 if (Input::GetKeyPress('D')) { /*mover->transform->Position.x += 0.1f;*/ directionX =  scene->GetCamera()->camera->GetRight();		}
					else if (Input::GetKeyPress('A')) { /*mover->transform->Position.x -= 0.1f;*/ directionX = -scene->GetCamera()->camera->GetRight();		}
				}
				else
				{
						 if (Input::GetKeyPress('W')) { /*mover->transform->Position.z += 0.1f;*/ directionZ = mover->transform->GetRightDirection();		}
					else if (Input::GetKeyPress('S')) { /*mover->transform->Position.z -= 0.1f;*/ directionZ = -mover->transform->GetRightDirection();		}
						 if (Input::GetKeyPress('Q')) { /*mover->transform->Position.y += 0.1f;*/ directionY = mover->transform->GetUpDirection();			}
					else if (Input::GetKeyPress('E')) { /*mover->transform->Position.y -= 0.1f;*/ directionY = -mover->transform->GetUpDirection();			}
						 if (Input::GetKeyPress('D')) { /*mover->transform->Position.x += 0.1f;*/ directionX = -mover->transform->GetForwardDirection();	}
					else if (Input::GetKeyPress('A')) { /*mover->transform->Position.x -= 0.1f;*/ directionX = mover->transform->GetForwardDirection();		}
				}

				directionX.y = 0.0f;
				directionZ.y = 0.0f;

				directionY.x = 0.0f;
				directionY.z = 0.0f;

				D3DXVECTOR3 direction = directionX + directionY + directionZ;
				D3DXVec3Normalize(&direction, &direction);

				switch (edit)
				{
				case EDIT_MODE::POSITION:	speed = 0.1f; mover->transform->Position += direction * speed;	break;
				case EDIT_MODE::ROTATION:	speed = 0.5f; mover->transform->Rotation += direction * speed;	break;
				case EDIT_MODE::SCALE:		speed = 0.01f; mover->transform->Scale += direction * speed;	break;
				default: break;
				}
			}
			vector[index]->EngineDisplay();

			for (auto component : vector[index]->GetComponentList())
			{
				ImGui::SeparatorText("");
				ImGui::SameLine();

				ImGui::Checkbox(" ", component->GetEnabledPointer());

				ImGui::SameLine();
				component->EngineDisplay();
			}
		}
	}

	ImGui::End();

	ImGui::Begin("Edit Mode", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	int ret = edit;
	ImGui::RadioButton("Move", &ret, 0); ImGui::SameLine();
	ImGui::RadioButton("Rotate", &ret, 1); ImGui::SameLine();
	ImGui::RadioButton("Scale", &ret, 2); ImGui::SameLine();

	edit = (EDIT_MODE)ret;

	ImGui::End();

	ImGui::Begin("", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (ImGui::Button(">"))
	{
		if (play == false)
		{
			Time::timeScale = 1.0f;
			play = true;
		}
		else
		{
			Time::timeScale = 0.0f;
			play = false;
			Manager::ResetScene();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("||"))
	{
		if (paused == true)
		{
			if (play == true) { Time::timeScale = 1.0f; }
			paused = false;
		}
		else
		{
			if (play == true) { Time::timeScale = 0.0f; }
			paused = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button(">|"))
	{
		if (play == true)
		{
			if (paused == true)
			{
				Time::timeScale = 1.0f;
			}
		}
	}
	else
	{
		if (play == true)
		{
			if (paused == true)
			{
				Time::timeScale = 0.0f;
			}
		}
	}

	ImGui::End();
}

bool DebugManager::BoolDisplay(bool* value, float offset, const char* text, int index, bool uneditable)
{
	int reference = (*value ? 1 : 0);

	ImGui::PushItemWidth(offset);
	ImGui::PushID(index);

	if (*value == false) 
	{ 
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); 
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
	}
	else 
	{ 
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
	}

	ImGui::SliderInt("", &reference, 0, 1, text);

	if (uneditable == false) { *value = (reference == 0 ? false : true); }

	ImGui::PopStyleColor(2);
	ImGui::PopID();

	return *value;
}

float DebugManager::FloatDisplay(float* value, float offset, const char* text, bool drag, D3DXVECTOR2 speedLimit, int index, bool uneditable)
{
	char str[40];
	sprintf_s(str, sizeof(str), "%s : %.2f", text, *value);

	float reference = *value;

	ImGui::PushItemWidth(offset);
	ImGui::PushID(index);

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));

	if (drag == false) { ImGui::SliderFloat("", &reference, speedLimit.x, speedLimit.y, str); }
	else { ImGui::DragFloat("", &reference, speedLimit.x, 0.0F, 0.0F, str); }

	if (uneditable == false) { *value = reference; }

	ImGui::PopStyleColor(2);
	ImGui::PopID();

	return *value;
}

D3DXVECTOR3 DebugManager::Float3Display(D3DXVECTOR3* value, float offset, const char* text, float speed, int index, bool uneditable)
{
	D3DXVECTOR3 reference = *value;

	ImGui::PushItemWidth(offset);
	ImGui::Text(text);
	ImGui::SameLine();

	ImGui::PushID(index);

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));

	ImGui::DragFloat3("", reference, speed);

	if (uneditable == false) { *value = reference; }

	ImGui::PopStyleColor(2);
	ImGui::PopID();

	return *value;
}