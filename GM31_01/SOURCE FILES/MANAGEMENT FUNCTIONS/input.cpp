#include "input.h"
#include "InputController.h"
#include "debugManager.h"

bool Input::controls = true;
bool Input::controller = false;

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];

std::vector<BYTE> Input::input[KEYMAPPING_MAX];
std::vector<WORD> Input::controllerInput[KEYMAPPING_MAX];

void Input::Init()
{
	controls = true;

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

	input[ROLL_KEYMAP].push_back(VK_SPACE);
	controllerInput[ROLL_KEYMAP].push_back(XINPUT_GAMEPAD_A);

	input[LIGHT_ATTACK_KEYMAP].push_back('Z');
	controllerInput[LIGHT_ATTACK_KEYMAP].push_back(XINPUT_GAMEPAD_X);

	input[HEAVY_ATTACK_KEYMAP].push_back('X');
	controllerInput[HEAVY_ATTACK_KEYMAP].push_back(XINPUT_GAMEPAD_Y);

	input[CHANGE_KEYMAP].push_back(VK_RETURN);
	controllerInput[CHANGE_KEYMAP].push_back(XINPUT_GAMEPAD_BACK);

	input[PAUSE_KEYMAP].push_back('P');
	controllerInput[PAUSE_KEYMAP].push_back(XINPUT_GAMEPAD_START);

	//controllerInput[AIM_KEYMAP].push_back(XINPUT_GAMEPAD_LEFT_THUMB);

	//controllerInput[SHOOT_KEYMAP].push_back(XINPUT_GAMEPAD_RIGHT_THUMB);
}

void Input::Uninit()
{


}

void Input::Update()
{
	UpdateXinput();

	memcpy(m_OldKeyState, m_KeyState, 256);

	GetKeyboardState(m_KeyState);

}

bool Input::GetKeyPress(BYTE KeyCode)
{
	if (controls == true) { return (m_KeyState[KeyCode] & 0x80); }
	else { return false; }
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	if (controls == true) { return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80)); }
	else { return false; }
}

bool Input::GetButtonPress(KEYMAPPING value)
{
	bool v = false;

	if (controls == true)
	{
		for (int i = 0; i < input[value].size(); i++)
		{
			v = GetKeyPress(input[value][i]);
			if (v == true) { return v; }
		}

		if (value == AIM_KEYMAP)
		{
			v = IsMouseRightPressed();
			if (v == true) { return true; }

			v = GetControllerLeftTrigger();
			if (v == true) { return true; }
		}
		else if (value == SHOOT_KEYMAP)
		{
			v = IsMouseLeftPressed();
			if (v == true) { return true; }

			v = GetControllerRightTrigger();
			if (v == true) { return true; }
		}

		for (int i = 0; i < controllerInput[value].size(); i++)
		{
			v = GetControllerButtonPress(controllerInput[value][i]);
			if (v == true) { return v; }
		}

	}

	return v;
}

bool Input::GetButtonTrigger(KEYMAPPING value)
{
	bool v = false;

	if (controls == true)
	{
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
	}

	return v;
}

float Input::Horizontal()
{
	float horizontal = 0.0f;

	if (controls == true)
	{
		horizontal = GetLeftJoyStick().x;
		if (fabs(horizontal) < 0.2f) { horizontal = 0.0f; }

		if (GetButtonPress(LEFT_KEYMAP)) { horizontal = -1.0f; }
		else if (GetButtonPress(RIGHT_KEYMAP)) { horizontal = 1.0f; }
	}

	return horizontal;
}

float Input::Vertical()
{
	float vertical = 0.0f;

	if (controls == true)
	{
		vertical = GetLeftJoyStick().y;
		if (fabs(vertical) < 0.2f) { vertical = 0.0f; }

		if (GetButtonPress(FORWARD_KEYMAP)) { vertical = 1.0f; }
		else if (GetButtonPress(BACK_KEYMAP)) { vertical = -1.0f; }
	}

	return vertical;
}

float Input::CameraAngleHorizontal()
{
	float horizontal = 0.0f;

	if (controls == true)
	{
		horizontal = GetRightJoyStick().x;

		//horizontal = GetMousePosX();

	}

	return horizontal;
}

float Input::CameraAngleVertical()
{
	float vertical = 0.0f;

	if (controls == true)
	{
		vertical = GetRightJoyStick().y;

		//vertical = GetMousePosY();

	}

	return vertical;
}