#pragma once
#include "../assimp/matrix4x4.h"

class Mathf
{
public:

	static float Lerp(float a, float b, float time)
	{
		return ((b - a) * time + a);
	}

	static D3DXMATRIX MatrixConvert(aiMatrix4x4 aiMatrix)
	{
		D3DXMATRIX fuse;

		fuse._11 = aiMatrix.a1;
		fuse._12 = aiMatrix.b1;
		fuse._13 = aiMatrix.c1;
		fuse._14 = aiMatrix.d1;

		fuse._21 = aiMatrix.a2;
		fuse._22 = aiMatrix.b2;
		fuse._23 = aiMatrix.c2;
		fuse._24 = aiMatrix.d2;

		fuse._31 = aiMatrix.a3;
		fuse._32 = aiMatrix.b3;
		fuse._33 = aiMatrix.c3;
		fuse._34 = aiMatrix.d3;

		fuse._41 = aiMatrix.a4;
		fuse._42 = aiMatrix.b4;
		fuse._43 = aiMatrix.c4;
		fuse._44 = aiMatrix.d4;


		return fuse;
	}
};

class Vector3
{
public:

	static D3DXVECTOR3 Lerp(D3DXVECTOR3 a, D3DXVECTOR3 b, float time)
	{
		return ((b - a) * time + a);
	}

	static float Magnitude(D3DXVECTOR3 value)
	{
		float x = value.x;
		float y = value.y;
		float z = value.z;

		float mag = sqrtf(x * x + y * y + z * z);
		return mag;
	}
};

class Vector4
{
public:

	static D3DXCOLOR Lerp(D3DXCOLOR a, D3DXCOLOR b, float time)
	{
		return ((b - a) * time + a);
	}

	static D3DXVECTOR4 Lerp(D3DXVECTOR4 a, D3DXVECTOR4 b, float time)
	{
		return ((b - a) * time + a);
	}
};

class Time
{
public:
	static float timeScale;
	static float deltaTime;
	static float fixedTimeScale;

	static bool WaitForSeconds(float time, float* var)
	{
		*var += deltaTime;

		char str[20];
		sprintf_s(str, sizeof(str), "Timer : %.2f", *var);

#ifdef DEBUG
		//ImGui::Begin("Time"/*, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize*/);
		//ImGui::Text(str);
		//ImGui::End();
#endif

		if (*var > time) { return true; }
		else { return false; }
	}
};