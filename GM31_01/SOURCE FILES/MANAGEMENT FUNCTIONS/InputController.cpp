#include "InputController.h"


_XINPUT_STATE state;
_XINPUT_STATE oldState;

/*
XINPUT_GAMEPAD_X
XINPUT_GAMEPAD_Y
XINPUT_GAMEPAD_A
XINPUT_GAMEPAD_B
XINPUT_GAMEPAD_RIGHT_SHOULDER
XINPUT_GAMEPAD_LEFT_SHOULDER
XINPUT_GAMEPAD_LEFT_THUMB
XINPUT_GAMEPAD_LEFT_THUMB
XINPUT_GAMEPAD_START
XINPUT_GAMEPAD_BACK
XINPUT_GAMEPAD_DPAD_UP
XINPUT_GAMEPAD_DPAD_DOWN
XINPUT_GAMEPAD_DPAD_RIGHT
XINPUT_GAMEPAD_DPAD_LEFT
*/

void UpdateXinput()
{
	oldState = state;

	XInputGetState(0, &state);

	if ((state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		state.Gamepad.sThumbLX = 0;
		state.Gamepad.sThumbLY = 0;
	}
	if ((state.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(state.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		state.Gamepad.sThumbRX = 0;
		state.Gamepad.sThumbRY = 0;
	}

}

const D3DXVECTOR2& GetLeftJoyStick()
{
	return	D3DXVECTOR2((float)state.Gamepad.sThumbLX / 32768.0f, (float)state.Gamepad.sThumbLY / 32768.0f);
}

const	D3DXVECTOR2&	GetRightJoystick()
{
	return	D3DXVECTOR2((float)state.Gamepad.sThumbRX / 32768.0f, (float)state.Gamepad.sThumbRY / 32768.0f);
}

bool GetControllerButtonPress(WORD button)
{
	if (state.Gamepad.wButtons == button)
	{
		return true;
	}
	return false;
}

bool GetControllerButtonTrigger(WORD button)
{
	if (state.Gamepad.wButtons == button && oldState.Gamepad.wButtons != button)
	{
		return true;
	}
	return false;
}