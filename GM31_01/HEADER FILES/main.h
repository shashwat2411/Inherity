#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>


#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>

//ImGUI
#include "debugManager.h"

#pragma warning(pop)

#define DEBUG
#define FRAME_RATE 60.0f

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")


#define SCREEN_WIDTH	(960)
#define SCREEN_HEIGHT	(540)


HWND GetWindow();
char* GetDebugStr(void);

