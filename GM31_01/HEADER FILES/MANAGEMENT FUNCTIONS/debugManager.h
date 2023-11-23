#pragma once
#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_win32.h"
#include "../imGUI/imgui_impl_dx11.h"

class SCENE;

class DebugManager
{
public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void TransformDraw(SCENE* scene);
};