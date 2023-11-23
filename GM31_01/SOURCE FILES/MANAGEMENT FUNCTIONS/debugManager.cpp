#include "debugManager.h"
#include "main.h"
#include "renderer.h"
#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_dx11.h"
#include "../imGUI/imgui_impl_win32.h"
#include "manager.h"
#include "input.h"


bool show_demo_window = true;

int index = 0;
int layer = 0;

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

	// ImGuiコード
	//フレームの描画
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DebugManager::TransformDraw(SCENE * scene)
{
	std::vector<GAMEOBJECT*> vector = scene->GetGameObjectListVector((LAYER)layer);

	ImGui::BeginGroup();
	//ImGui::SetNextWindowSize(ImVec2(140, 75));

	ImGui::Begin("Index");
	ImGui::PushItemWidth(-FLT_MIN);
	ImGui::SliderInt(" ", &layer, 0, MAX_LAYER - 1, str[layer]);

	if (index > vector.size() - 1) { index = vector.size() - 1; }
	ImGui::PushItemWidth(-FLT_MIN);
	if (vector[0] != nullptr) { ImGui::SliderInt("", &index, 0, vector.size() - 1, vector[index]->GetTag().c_str()); }
	//ImGui::InputInt("Layer", &layer);

	//ImGui::SetNextWindowSize(ImVec2(200, 170));
	ImGui::Begin("Inspector");
	{
		if (vector[0] != nullptr)
		{
			vector[index]->EngineDisplay();

			for (auto component : vector[index]->GetComponentList())
			{
				ImGui::SeparatorText("");
				component->EngineDisplay();
			}
		}
	}

	ImGui::End();
}
