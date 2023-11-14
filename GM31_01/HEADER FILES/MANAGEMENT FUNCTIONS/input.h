#pragma once
#include "main.h"
#include <vector>

enum KEYMAPPING
{
	FORWARD_KEYMAP,
	BACK_KEYMAP,
	LEFT_KEYMAP,
	RIGHT_KEYMAP,

	JUMP_KEYMAP,
	CHANGE_KEYMAP,

	KEYMAPPING_MAX
};

class Input
{
public:

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
