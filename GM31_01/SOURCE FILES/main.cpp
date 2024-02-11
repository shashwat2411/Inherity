#include "manager.h"

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11ゲーム";

int		g_CountFPS;							// FPSカウンタ
int		g_CountFixedFPS;					// FixedFPSカウンタ
char	g_DebugStr[2048] = "";		// デバッグ文字表示用

long g_MouseX = 0;
long g_MouseY = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


HWND g_Window;

HWND GetWindow()
{
	return g_Window;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);


		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH * 1.0f, (LONG)SCREEN_HEIGHT * 1.0f };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW/* | WS_MAXIMIZE*/, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}

	Manager::Init();
	InitializeMouse(hInstance, g_Window);

	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);


	DWORD dwExecLastTime;
	DWORD dwExecFixedLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;

	DWORD dwFrameCount;
	DWORD dwFixedFrameCount;

	timeBeginPeriod(1);
	dwExecLastTime = dwExecFixedLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = dwFixedFrameCount = 0;



	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= (1000))
			{
				g_CountFPS = dwFrameCount;
				g_CountFixedFPS = dwFixedFrameCount;

				dwFPSLastTime = dwCurrentTime;

				dwFrameCount = 0;
				dwFixedFrameCount = 0;
			}

			float ab = 1000.0f / (FRAME_RATE * ((Time::timeScale > 0.0f) ? Time::timeScale : 1.0f/ FRAME_RATE));
			//if ((dwCurrentTime - dwExecFixedLastTime) >= (ab) && Time::timeScale > 0.0f)
			if ((dwCurrentTime - dwExecFixedLastTime) >= (1000 / FRAME_RATE))
			{
				wsprintf(g_DebugStr, "ENGINE| FPS : %d", g_CountFixedFPS);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());

				UpdateMouse();
				Manager::FixedUpdate();
				Manager::Draw();

				dwExecFixedLastTime = dwCurrentTime;
				dwFixedFrameCount++;

				SetWindowText(g_Window, g_DebugStr);
			}

			//if ((dwCurrentTime - dwExecLastTime) >= (1000 / FRAME_RATE))
			//{
			//	dwExecLastTime = dwCurrentTime;

			//	//Manager::Update();
			//	dwFrameCount++;
			//}

		}
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	UninitMouse();
	Manager::Uninit();

	return (int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

char* GetDebugStr(void)
{
	return g_DebugStr;
}

static LPDIRECTINPUTDEVICE8 pMouse = NULL; // mouse

static DIMOUSESTATE2   mouseState;		// マウスのダイレクトな状態
static DIMOUSESTATE2   mouseTrigger;	// 押された瞬間だけON

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow)
{
	HRESULT hr;
	LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ

	if (!g_pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	HRESULT result;
	// デバイス作成
	result = g_pDInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	if (FAILED(result) || pMouse == NULL)
	{
		MessageBox(hWindow, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// データフォーマット設定
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// 他のアプリと協調モードに設定
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// デバイスの設定
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// マウスの移動値　相対値

	result = pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// アクセス権を得る
	pMouse->Acquire();
	return result;
}

void UninitMouse()
{
	if (pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse = NULL;
	}

}

HRESULT UpdateMouse()
{
	HRESULT result;
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = mouseState;
	// データ取得
	result = pMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	if (SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// マウスのボタン状態
		for (int i = 0; i < 8; i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		result = pMouse->Acquire();
	}
	return result;

}

BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
}
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
}
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80);
}
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80);
}
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80);
}
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80);
}

//------------------
long GetMouseX(void)
{
	return mouseState.lX;
}
long GetMouseY(void)
{
	return mouseState.lY;
}
long GetMouseZ(void)
{
	return mouseState.lZ;
}

long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}