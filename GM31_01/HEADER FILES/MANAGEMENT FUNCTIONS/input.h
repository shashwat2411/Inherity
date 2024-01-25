#pragma once
#include <vector>

enum KEYMAPPING
{
	FORWARD_KEYMAP,
	BACK_KEYMAP,
	LEFT_KEYMAP,
	RIGHT_KEYMAP,

	ROLL_KEYMAP,
	LIGHT_ATTACK_KEYMAP,
	HEAVY_ATTACK_KEYMAP,
	CHANGE_KEYMAP,

	KEYMAPPING_MAX
};

class Input
{
public:

private:
	static bool controls;

	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

	static std::vector<BYTE> input[KEYMAPPING_MAX];
	static std::vector<WORD> controllerInput[KEYMAPPING_MAX];

public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress( BYTE KeyCode );
	static bool GetKeyTrigger( BYTE KeyCode );

	static bool GetButtonPress(KEYMAPPING value);
	static bool GetButtonTrigger(KEYMAPPING value);

	static float Horizontal();
	static float Vertical();

	static void SetControls(bool value) { controls = value; }
};
