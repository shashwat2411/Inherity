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

#define DIRECTINPUT_VERSION 0x0800		// 警告対処
#include <dinput.h>
#include "mmsystem.h"

#define IMGUI_DEFINE_MATH_OPERATORS

//ImGUI
#include "../imGUI/imgui.h"
#include "../imGUI/imgui_impl_win32.h"
#include "../imGUI/imgui_impl_dx11.h"
#include "../imGUI/implot.h"


#pragma warning(pop)

#define DEBUG
#define FRAME_RATE 60.0f

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")

#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")


#define SCREEN_WIDTH	(960)
#define SCREEN_HEIGHT	(540)


HWND GetWindow();
char* GetDebugStr(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow);
void UninitMouse();
HRESULT UpdateMouse();

BOOL IsMouseLeftPressed(void);      // 左クリックした状態
BOOL IsMouseLeftTriggered(void);    // 左クリックした瞬間
BOOL IsMouseRightPressed(void);     // 右クリックした状態
BOOL IsMouseRightTriggered(void);   // 右クリックした瞬間
BOOL IsMouseCenterPressed(void);    // 中クリックした状態
BOOL IsMouseCenterTriggered(void);  // 中クリックした瞬間
long GetMouseX(void);               // マウスがX方向に動いた相対値
long GetMouseY(void);               // マウスがY方向に動いた相対値
long GetMouseZ(void);               // マウスホイールが動いた相対値

long GetMousePosX(void);
long GetMousePosY(void);