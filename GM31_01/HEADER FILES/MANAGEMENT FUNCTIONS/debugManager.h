#pragma once
#include "main.h"

class SCENE;

enum EDIT_MODE
{
	POSITION,
	ROTATION,
	SCALE
};

class DebugManager
{
public:
	static bool play;
	static bool paused;
	static bool gizmo;
	static bool show_demo_window;

	static int index;
	static int layer;

	static float speed;

public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void DebugDraw(SCENE* scene);

	static bool BoolDisplay(bool* value, float offset, const char* text, int index = 0, bool uneditable = false);
	static float FloatDisplay(float* value, float offset, const char* text, bool drag, D3DXVECTOR2 speedLimit, int index = 0, bool uneditable = false);
	static D3DXVECTOR3 Float3Display(D3DXVECTOR3* value, float offset, const char* text, float speed, int index = 0, bool uneditable = false, float min = 0.0f, float max = 0.0f);
};