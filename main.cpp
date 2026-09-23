
/*==========================================================================

	main.cpp


													Author : hidetoshi muramatu
													Date   : 2026/
---------------------------------------------------------------------------



==========================================================================*/

#include "main.h"
#include "manager.h"
#include <thread>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "system_timer.h"
const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "やらない前戯よりやる前戯";


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

	(void)CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	SystemTimer_Initialize();
	Manager::Init();



	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);




	//DWORD dwExecLastTime;
	//DWORD dwCurrentTime;
	timeBeginPeriod(1);
	//dwExecLastTime = timeGetTime();
	//dwCurrentTime = 0;
	double exec_last_time = 0.0f;
	double fps_last_time = 0.0f;
	double current_time = 0.0f;
	ULONG frame_count = 0; // フレームカウント
	double fps = 0.0f; // フレームレート（FPS）計算用変数
	exec_last_time = fps_last_time = SystemTimer_GetTime(); // 最初の時間を取得


	MSG msg;
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
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
			current_time = SystemTimer_GetTime(); // 現在の時間を取得
			double elapsed_time = current_time - fps_last_time; // 前回の時間との差分を計算

			if (elapsed_time >= 1.0)
			{


				fps = frame_count / elapsed_time; // フレームレートを計算

				fps_last_time = current_time; // 最後の時間を更新


				frame_count = 0; // フレームカウントをリセット
			}

			elapsed_time = current_time - exec_last_time; // 前回の実行時間との差分を計算

			//dwCurrentTime = timeGetTime();

			if((elapsed_time) >= (1.0 / 60.0))
		    //if ((elapsed_time) >= (1.0 / 140.0))
			{
				//dwExecLastTime = dwCurrentTime;
				exec_last_time = current_time; // 最後の実行時間を更新

				Manager::Update(elapsed_time);
				Manager::Draw();
				frame_count++; // フレームカウントを増やす

			}
		}
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::Uninit();

	CoUninitialize();

	return (int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(
		hWnd,
		uMsg,
		wParam,
		lParam))
	{
		return true;
	}
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
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

