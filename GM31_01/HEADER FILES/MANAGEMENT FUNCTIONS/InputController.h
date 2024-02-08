#pragma once
#include	"main.h"
#include	<xinput.h>
#include	<Windows.h>
#pragma comment(lib,"xinput.lib")

void UpdateXinput();

const D3DXVECTOR2 GetLeftJoyStick();
const D3DXVECTOR2 GetRightJoyStick();

bool GetControllerButtonPress(WORD button);
bool GetControllerButtonTrigger(WORD button);

bool GetControllerLeftTrigger();
bool GetControllerRightTrigger();
