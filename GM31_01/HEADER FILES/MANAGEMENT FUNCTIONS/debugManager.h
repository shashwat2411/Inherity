#pragma once
#include "main.h"

class SCENE;

class DebugManager
{
public:

	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void TransformDraw(SCENE* scene);

	static bool BoolDisplay(bool* value, float offset, const char* text, int index = 0, bool uneditable = false);
	static float FloatDisplay(float* value, float offset, const char* text, bool drag, D3DXVECTOR2 speedLimit, int index = 0, bool uneditable = false);
	static D3DXVECTOR3 Float3Display(D3DXVECTOR3* value, float offset, const char* text, float speed, int index = 0, bool uneditable = false);
};