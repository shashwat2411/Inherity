#pragma once

#ifndef XINPUT_H_
#include	"main.h"
#include	<xinput.h>
#include	<Windows.h>
#pragma comment(lib,"xinput.lib")

void UpdateXinput();

const D3DXVECTOR2&	GetLeftJoyStick();
const D3DXVECTOR2&	GetRightJoystick();

bool GetControllerButtonPress(WORD button);
bool GetControllerButtonTrigger(WORD button);