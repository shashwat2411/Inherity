#include "input.h"
#include "InputController.h"

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];

std::vector<BYTE> Input::input[KEYMAPPING_MAX];
std::vector<WORD> Input::controllerInput[KEYMAPPING_MAX];

void Input::Init()
{
	memset( m_OldKeyState, 0, 256 );
	memset( m_KeyState, 0, 256 );

	input[FORWARD_KEYMAP].push_back('W');
	input[FORWARD_KEYMAP].push_back(VK_UP);
	controllerInput[FORWARD_KEYMAP].push_back(XINPUT_GAMEPAD_DPAD_UP);

	input[BACK_KEYMAP].push_back('S');
	input[BACK_KEYMAP].push_back(VK_DOWN);
	controllerInput[BACK_KEYMAP].push_back(XINPUT_GAMEPAD_DPAD_DOWN);

	input[LEFT_KEYMAP].push_back('A');
	input[LEFT_KEYMAP].push_back(VK_LEFT);
	controllerInput[LEFT_KEYMAP].push_back(XINPUT_GAMEPAD_DPAD_LEFT);

	input[RIGHT_KEYMAP].push_back('D');
	input[RIGHT_KEYMAP].push_back(VK_RIGHT);
	controllerInput[RIGHT_KEYMAP].push_back(XINPUT_GAMEPAD_DPAD_RIGHT);

	input[JUMP_KEYMAP].push_back(VK_SPACE);
	controllerInput[JUMP_KEYMAP].push_back(XINPUT_GAMEPAD_A);

	input[CHANGE_KEYMAP].push_back(VK_RETURN);
	controllerInput[CHANGE_KEYMAP].push_back(XINPUT_GAMEPAD_START);
}

void Input::Uninit()
{


}

void Input::Update()
{
	UpdateXinput();

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

	for (int i = 0; i < controllerInput[value].size(); i++)
	{
		v = GetControllerButtonPress(controllerInput[value][i]);
		if (v == true) { return v; }
	}

	return false;
}

bool Input::GetButtonTrigger(KEYMAPPING value)
{
	bool v = false;

	for (int i = 0; i < input[value].size(); i++)
	{
		v = GetKeyTrigger(input[value][i]);
		if (v == true) { return v; }
	}

	for (int i = 0; i < controllerInput[value].size(); i++)
	{
		v = GetControllerButtonTrigger(controllerInput[value][i]);
		if (v == true) { return v; }
	}
	return false;
}

float Input::Horizontal()
{
	float horizontal = 0.0f;

	horizontal = GetLeftJoyStick().x;

	if (GetButtonPress(LEFT_KEYMAP)) { horizontal = -1.0f; }
	else if (GetButtonPress(RIGHT_KEYMAP)) { horizontal = 1.0f; }

	return horizontal;
}

float Input::Vertical()
{
	float vertical = 0.0f;

	vertical = GetLeftJoyStick().y;

	if (GetButtonPress(FORWARD_KEYMAP)) { vertical = 1.0f; }
	else if (GetButtonPress(BACK_KEYMAP)) { vertical = -1.0f; }

	return vertical;
}