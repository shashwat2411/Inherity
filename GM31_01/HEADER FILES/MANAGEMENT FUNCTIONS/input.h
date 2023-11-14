#pragma once
#include "main.h"
#include <vector>

class vector;

class Input
{
public:

	enum KEYMAPPING
	{
		QK_FORWARD,

		KEYMAPPING_MAX
	};

private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

	static std::vector<BYTE> input[KEYMAPPING_MAX];

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

	static bool GetButtonPress(KEYMAPPING value);
	static bool GetButtonTrigger(KEYMAPPING value);
};
