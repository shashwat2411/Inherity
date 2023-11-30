#include "main.h"
#include "manager.h"
#include "functions.h"

#include <thread>


const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11ゲーム";

#ifdef DEBUG
int		g_CountFPS;							// FPSカウンタ
int		g_CountFixedFPS;					// FixedFPSカウンタ
char	g_DebugStr[2048] = "TRIAL";		// デバッグ文字表示用

#endif

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


		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}


	Manager::Init();


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
#ifdef DEBUG
				wsprintf(g_DebugStr, "ENGINE| FPS : %d | FixedFPS : %d | ", g_CountFPS, g_CountFixedFPS);
#endif

				Manager::FixedUpdate();
				Manager::Draw();

				dwExecFixedLastTime = dwCurrentTime;
				dwFixedFrameCount++;

#ifdef DEBUG
				SetWindowText(g_Window, g_DebugStr);
#endif
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

	Manager::Uninit();

	return (int)msg.wParam;
}

#ifdef DEBUG
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

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

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#ifdef DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif