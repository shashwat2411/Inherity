#include "debugManager.h"
#include "main.h"
#include "renderer.h"
#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_dx11.h"
#include "../imGUI/imgui_impl_win32.h"


bool show_demo_window = true;

void DebugManager::Init()
{
	//ImGui::CreateContext();
	//ImGui_ImplWin32_Init(GetWindow());
	//ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

		// Setup Dear ImGui context
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

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);

	//ImGui::BeginGroup();
	//// ImGuiウィンドウやコントロールの作成と更新
	//ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_Once);
	//ImGui::Begin("hoge", ((bool*)0), ImGuiWindowFlags_NoResize);
	//ImGui::Text("fugafuga");
	//ImGui::End();


	// ImGuiコード
	//フレームの描画
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}