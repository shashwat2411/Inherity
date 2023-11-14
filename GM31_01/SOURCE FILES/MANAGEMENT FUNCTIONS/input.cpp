#include "input.h"

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];

std::vector<BYTE> Input::input[KEYMAPPING_MAX];

void Input::Init()
{
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

	input[FORWARD_KEYMAP].push_back('W');
	input[FORWARD_KEYMAP].push_back(VK_UP);
	input[FORWARD_KEYMAP].push_back(VK_GAMEPAD_LEFT_THUMBSTICK_UP);

	input[BACK_KEYMAP].push_back('S');
	input[BACK_KEYMAP].push_back(VK_DOWN);
	input[BACK_KEYMAP].push_back(VK_GAMEPAD_LEFT_THUMBSTICK_DOWN);

	input[LEFT_KEYMAP].push_back('A');
	input[LEFT_KEYMAP].push_back(VK_LEFT);
	input[LEFT_KEYMAP].push_back(VK_GAMEPAD_LEFT_THUMBSTICK_LEFT);

	input[RIGHT_KEYMAP].push_back('D');
	input[RIGHT_KEYMAP].push_back(VK_RIGHT);
	input[RIGHT_KEYMAP].push_back(VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT);

	input[JUMP_KEYMAP].push_back(VK_SPACE);
	input[JUMP_KEYMAP].push_back(VK_GAMEPAD_A);

	input[CHANGE_KEYMAP].push_back(VK_RETURN);
	input[CHANGE_KEYMAP].push_back(VK_GAMEPAD_MENU);
}

void Input::Uninit()
{


}

void Input::Update()
{

	memcpy( m_OldKeyState, m_KeyState, 256 );

	GetKeyboardState( m_KeyState );

}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool Input::GetButtonPress(KEYMAPPING value)
{
	bool v = false;

	for (int i = 0; i < input[value].size(); i++)
	{
		v = GetKeyPress(input[value][i]);
		if (v == true) { return v; }
	}
	return v;
}

bool Input::GetButtonTrigger(KEYMAPPING value)
{
	bool v = false;

	for (int i = 0; i < input[value].size(); i++)
	{
		v = GetKeyTrigger(input[value][i]);
		if (v == true) { return v; }
	}
	return v;
}
